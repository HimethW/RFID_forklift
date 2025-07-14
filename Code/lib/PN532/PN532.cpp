/*
    PN532.h and MIFARE_Classic_Card.h

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Provides an interface to communicate with NXP's PN532 RFID module over SPI, and authenticate, read, and write blocks of MIFARE
    Classic Cards.

    The following sources were referenced.

    https://github.com/adafruit/Adafruit-PN532/blob/bf9f3f31961474fdbca23cb57d604c7f27269baf/Adafruit_PN532.h
    https://github.com/adafruit/Adafruit-PN532/blob/bf9f3f31961474fdbca23cb57d604c7f27269baf/Adafruit_PN532.cpp

    https://www.nxp.com/docs/en/user-guide/141520.pdf [PN532UM]
    https://www.nxp.com/docs/en/nxp/data-sheets/PN532_C1.pdf [PN532DS]
    http://www.emutag.com/iso/14443-3.pdf [ISO-3]

    https://www.nxp.com/docs/en/data-sheet/MF1S50YYX_V1.pdf
*/

#include "Pins.h"
#include "SPI.h"
#include "Timer.h"
#include "PN532_Commands.h"
#include "MIFARE_Classic_Commands.h"
#include "PN532.h"

#include "string.h"

/*
    PN532 Methods
*/

PN532::PN532(Pin NSS, Pin MOSI, Pin MISO, Pin SCK) : _NSS(NSS), _spi(MOSI, MISO, SCK) {
    _NSS.set_output();
};

void PN532::initialize() {
    _spi.initialize(LSB_FIRST);

    _NSS.assert(); // Keep the chip deactivated initially
    blocking_delay(5, MILLISECONDS);
};

bool PN532::send_bytes(unsigned char* bytes, int length) {
    return _spi.send(bytes, length);
};

bool PN532::receive_bytes(unsigned char* buffer, int length) {
    return _spi.receive(buffer, length);
};

bool PN532::write_frame(unsigned char* frame, int length) {
    /*
        Send a DATA_WRITE byte first, then send the bytes contained in `frame`
    */
    
    // NSS assertion and deassertion as described in Section 8.3.5.5 (PN532DS)
    _NSS.deassert();
    blocking_delay(5, MILLISECONDS);

    // Start by first sending a DATA_WRITE byte, as required by modified SPI frames [Section 6.2.5 (PN532UM)]
    if (!_spi.send_and_receive_byte(DATA_WRITE, nullptr)) {
        _NSS.assert();
        return false;
    }

    // Send the rest of the bytes
    if (!send_bytes(frame, length)) {
        _NSS.assert();
        return false;
    }

    _NSS.assert();
    blocking_delay(5, MILLISECONDS);

    return true;
};

bool PN532::read_frame(unsigned char* frame_target, int length, bool start, bool conclude) {
    /*
        If the PN532 has data available to be read, send a DATA_READ byte, then read `length` bytes of the data

        `start`     = 1 : we are just beginning to read a frame; select the PN532, and send a DATA_READ byte
        `start`     = 0 : we have begun reading a frame; PN532 is already selected and a DATA_READ byte has already been sent;
                          simply buffer in the next `length` bytes

        `conclude`  = 1 : we conclude receiving the response after reading the next `length` bytes; reassert NSS to deselect
                          the PN532 after reading the next `length` bytes
        `conclude`  = 0 : we are not reading the complete response, read only the next `length` bytes, but the response contains
                          more bytes; we intend to read them later; do not reassert NSS
    */

    // If not continuing from a previous frame read, select the PN532, and send a DATA_READ byte to prepare to read
    if (start) {
        // Deassert NSS to start data read [Section 8.3.5.4 (PN532DS)]
        _NSS.deassert();
        blocking_delay(5, MILLISECONDS);
        
        // Start by sending a DATA_READ byte [Section 6.2.5 (PN532UM)]
        if (!_spi.send_and_receive_byte(DATA_READ, nullptr)) {
            _NSS.assert();
            return false;
        }
    }

    // Read `length` bytes of the response
    if (!receive_bytes(frame_target, length)) {
        _NSS.assert();
        return false;
    }

    // If concluding the frame read, reassert NSS to deselect the PN532 [Section 8.3.5.4 (PN532DS)], else return without
    // asserting NSS
    if (conclude) {
        _NSS.assert();
    }

    return true;
};

void PN532::make_normal_information_frame(unsigned char* target_frame, unsigned char TFI, unsigned char* bytes, unsigned char num_bytes) {
    /*
        Prepare a frame appending the header and trailer to the given data bytes and put it in `target_frame`

        Frame Structure is
        
        PREAMBLE STARTCODE1 STARTCODE2 LEN LCS TFI PD1 ... PDn DCS POSTAMBLE

        PD1, ..., PDn   =   n bytes of data, with PD1 being the command code
        LEN             =   number of bytes including TFI and all the data bytes (number of data bytes + 1)
        LCS             =   Packet Length Checksum, such that the least significant byte of (LEN + LCS) = 0x00 (i.e., all 8 least
                            ssignificant bits of (LEN + LCS) are 0000 0000), equivalent to requiring (LEN + LCS) MOD 256 = 0; note '+'
                            is arithmetic summation, not boolean OR
        TFI             =   Target Frame Identifier, describes direction of data (PN532 to Host vs Host to PN532)
        DCS             =   Data Checksum, such that the least significant byte of (TFI + PD0 + ... + PDn + DCS) = 0x00

        [Section 6.2.1.1 (PN532UM)]
    */

    target_frame[PREAMBLE_IDX] = PREAMBLE;
    target_frame[STARTCODE1_IDX] = STARTCODE1;
    target_frame[STARTCODE2_IDX] = STARTCODE2;
    target_frame[LEN_IDX] = num_bytes + 1; // + 1 for TFI
    target_frame[LCS_IDX] = ~target_frame[LEN_IDX] + 1;
    target_frame[TFI_IDX] = TFI;
    
    unsigned char DCS = TFI;

    for (int i = 0; i < num_bytes; i++) {
        target_frame[TFI_IDX + 1 + i] = bytes[i];
        DCS += bytes[i];
    }

    target_frame[TFI_IDX + 1 + num_bytes] = ~DCS + 1;
    target_frame[TFI_IDX + 1 + num_bytes + 1] = POSTAMBLE;
};

bool PN532::ready_to_respond() {
    /*
        Poll the PN532's status byte until it has data available to be read
    */

    int timeout = 1000; // Timeout in milliseconds
    bool ready = false;

    // NSS assertion and deassertion as described in Section 8.3.5.3 (PN532DS)
    while (!(ready) && timeout > 0) {
        _NSS.deassert();

        blocking_delay(10, MILLISECONDS);
        timeout -= 10; 

        // Poll the Status byte and receive a byte of response [Section 6.2.5.1 (PN532UM)]
        _spi.send_and_receive_byte(STATUS_READ, nullptr);

        unsigned char response_buffer;
        _spi.send_and_receive_byte(0x00, &response_buffer);

        _NSS.assert();

        ready = (response_buffer & 0b1); // Extract the LSB of the received byte
    }

    return ready;
};

bool PN532::check_ack() {
    /*
        Check if the PN532 ACK'ed the previous command issued, by buffering in ACK_SIZE bytes of response, given that the PN532 is
        ready to respond
    */

    unsigned char response[ACK_SIZE];
    if (!read_frame(response, ACK_SIZE, true, true)) {
        return false;
    }

    for (int i = 0; i < 6; i++) {
        if (response[i] != ACK_FRAME[i]) {
            return false;
        }
    }

    return true;
};

bool PN532::issue_command_from_array(unsigned char* command_array, int length) {
    /*
        Send `command_array`, where the 0th entry is the command code, and all following entries contain the relevant parameters for
        the command, and wait for it to be acknowledged (`command_array` should contain PD1 ... PDn)
    */

    // Create a normal information frame by adding the frame header and trailer [Section 6.2.1.1 (PN532UM)]
    unsigned char normal_information_frame[FRAME_HEADER_SIZE + length + FRAME_TRAILER_SIZE];
    make_normal_information_frame(normal_information_frame, TFI_HOST_TO_PN532, command_array, length);

    if (!write_frame(normal_information_frame, FRAME_HEADER_SIZE + length + FRAME_TRAILER_SIZE)) {
        return false;
    }
    
    if (!ready_to_respond()) {
        return false;
    }

    return check_ack();
}

bool PN532::receive_command_response(unsigned char* response_buffer, int length, bool start, bool conclude) {
    /*
        See if the PN532 is ready to respond, then buffer in `length` bytes of the response

        `start` and `conclude` apply in the same sense as described in `read_frame()`
    */

    if (start) {
        if (!ready_to_respond()) {
            return false;
        }
    }

    return read_frame(response_buffer, length, start, conclude);
};

bool PN532::SAMConfig() {
    /*
        Configure the PN532 in the Normal Mode to not use SAM
        
        Command format is as follows;
        
        SAM_CONFIGURATION Mode Timeout IRQ

        Mode    = we use Normal Mode, no SAM
        Timeout = timeout after TIMEOUT * 50ms; we set TIMEOUT = 0x14 = 20, for 20 * 50 ms = 1 second timeout
        IRQ     = using IRQ pin (0x1) or not (0x0); we will not use IRQ

        [Section 7.2.10 (PN532UM)]
    */

    if (!issue_command(SAM_CONFIGURATION, 0x01, 0x14, 0x00)) {
        return false;
    }

    // Response is just OPCODE+1
    unsigned char response[FRAME_HEADER_SIZE + 1 + FRAME_TRAILER_SIZE];
    if (!receive_command_response(response, FRAME_HEADER_SIZE + 1 + FRAME_TRAILER_SIZE, true, true)) {
        return false;
    }

    return ((response[TFI_IDX] == TFI_PN532_TO_HOST) && (response[OPCODE_IDX] == SAM_CONFIGURATION + 1));
};

bool PN532::detect_card(unsigned char* card_number, unsigned char* card_data) {
    /*
        Find a tag, put its logical number in `tag_number`, read its UID (and ATS if ISO 14443-4 Compliant), and put it in `tag_data`

        Command format is;
        
        LIST_PASSIVE_TARGETS MaxTg BrTy

        MaxTg   = number of targets to be initialized; must be <= 2, we set to 1
        BrTy    = baud rate and modulation type; we set to 0x00 for ISO 14443 Type A

        [Section 7.3.5 (PN532UM)]
    */

    if (!issue_command(LIST_PASSIVE_TARGETS, 0x01, 0x00)) {
        return false;
    }

    // Response begins OPCODE+1 NbTg ...
    unsigned char response[FRAME_HEADER_SIZE + 2];
    if (!receive_command_response(response, FRAME_HEADER_SIZE + 2, true, false)) {
        return false;
    }
    // `response[7]` = NbTg is the number of tags detected; we do not need this value, we expect it to be 1 always, as we will
    // issue commands to read only 1 tag

    // Response continues from above as ... Tg ATQA_MSB ATQA_LSB SAK UID_Length ...
    if (!receive_command_response(response, 5, false, false)) {
        return false;
    };

    *card_number = response[0];
    
    card_data[ATQA_MSB_IDX] = response[1];
    card_data[ATQA_LSB_IDX] = response[2];
    
    unsigned char sak = response[3];
    card_data[SAK_IDX] = sak;

    int uid_length = response[4];
    card_data[UID_LEN_IDX] = uid_length;

    // As per Table 8, Section 6.4.3.4 (ISO-3)
    bool iso14443_4_compliant = sak & 0b100000;

    // Next `uid_length` bytes of the response form the UID of the card
    // If the card is not ISO14443-4 compliant, the response ends after the UID; there will be no ATS
    if (!receive_command_response(response, uid_length, false, !iso14443_4_compliant)) {
        return false;
    }

    // 4-byte UID goes in `tag_data[4]` ... `tag_data[7]`
    // 7-byte UID goes in `tag_data[4]` ... `tag_data[10]`
    // 10-byte UID goes in `tag_data[4]` ... `tag_data[13]`
    memcpy(card_data + 4, response, uid_length);

    if (iso14443_4_compliant) { // There are more bytes (ATS) to be read
        // Response continues as ... ATS_Length ...
        unsigned char ats_length;
        _spi.send_and_receive_byte(0x00, &ats_length);

        // Put ATS Length in `tag_data[4 + i]`
        card_data[4 + uid_length] = ats_length;

        // The next `ats_length` bytes form the ATS sent by the card; read them and conclude reading the frame
        if (!receive_command_response(response, ats_length, false, true)) {
            return false;
        }

        // Place the ATS bytes in the following indices
        memcpy(card_data + 4 + uid_length + 1, response, ats_length);
    }

    return true;
};

MIFARE_Classic_PN532* PN532::get_mifare_classic_card() {
    /*
        Use `detect_card()` to find a MIFARE Classic Card, and return a pointer to a new MIFARE_Classic_PN532 object
    */

    unsigned char card_number;
    unsigned char card_data[8];

    if (!detect_card(&card_number, card_data)) {
        return nullptr;
    } else {
        return new MIFARE_Classic_PN532(this, card_data + UID_START_IDX, card_data[UID_LEN_IDX]);
    }
};

/*
    MIFARE_Classic_PN532
*/

MIFARE_Classic_PN532::MIFARE_Classic_PN532(PN532* pn532_pcd, unsigned char* uid, int length) {
    _pcd = pn532_pcd;

    _uid_length = length;
    _uid = uid;
};

bool MIFARE_Classic_PN532::issue_command_from_array(unsigned char* command_array, int length) {
    return _pcd->issue_command_from_array(command_array, length);
};

bool MIFARE_Classic_PN532::receive_command_response(unsigned char* response_buffer, int length) {
    /*
        The response will always start
        
        OPCODE+1 Status ...
        
        and have `length` (at most 16) bytes sent by the MIFARE Classic Card following it [Section 7.3.8 (PN532UM)]

        If the command executed successfully, Status = 0 [Section 7.1 (PN532UM)]
    */

    response_buffer -= (FRAME_HEADER_SIZE + 2); // To accomodate the frame header, OPCODE+1, and Status bytes
    if (!_pcd->receive_command_response(response_buffer, FRAME_HEADER_SIZE + 2 + length + FRAME_TRAILER_SIZE, true, true)) {
        return false;
    }

    if (response_buffer[7] != 0) {
        return false;
    }
    
    return true;
};

bool MIFARE_Classic_PN532::executed_successfully() {
    /*
        Read just two bytes and see if Status = 0; use when no bytes sent by the MIFARE Classic Card are sent back to the host
    */

    unsigned char response[FRAME_HEADER_SIZE + 2 + FRAME_TRAILER_SIZE];
    if (!_pcd->receive_command_response(response, FRAME_HEADER_SIZE + 2 + FRAME_TRAILER_SIZE, true, true)) {
        return false;
    }

    if (response[7] != 0) {
        return false;
    }
    
    return true;
}

bool MIFARE_Classic_PN532::authenticate_block(unsigned char authentication_type, unsigned char block_address, unsigned char* key) {
    /*
        Authenticate a block on 4-byte UID card using the given 6-byte key

        Section 7.3.8 (PN532UM) describes format of the command to be issued to authenticate a card:
        
        DATA_EXCHANGE Tg AUTH_COMMAND Addr Key[0] ... Key[5] UID[0] ... UID[3]

        Tg                  = logical number of the tag to be authenticated, we set to 1
        AUTH_COMMAND        = MIFARE Classic command code; shall be either AUTHENTICATE_KEY_A or AUTHENTICATE_KEY_B; specifies which
                              type of key to use for authentication, pass this to the function in `authentication_type`
        Addr                = the address of the block to be authenticated, pass in `block_address`
        Key[0] ... Key[5]   = 6 bytes of the key to use for authentication, passed in `key`
        UID[0] ... UID[3]   = 4-byte UID of the card; already stored in `_uid`
    */

    unsigned char command_array[14];

    command_array[0] = DATA_EXCHANGE;
    command_array[1] = 1;
    command_array[2] = authentication_type;
    command_array[3] = block_address;

    memcpy(command_array + 4, key, 6);
    memcpy(command_array + 10, _uid, 4);

    if (!_pcd->issue_command_from_array(command_array, 14)) {
        return false;
    }

    // The PN532 does not send anything received from the PICC back to the host, simply check for successful execution
    return executed_successfully();
};

bool MIFARE_Classic_PN532::read_block(unsigned char block_address, unsigned char* contents) {
    /*
        Read the block at `block_address` of a 4-byte UID card, and place the 16 bytes of content there in `contents`

        Section 7.3.8 (PN532UM) describes format of the command:
        
        DATA_EXCHANGE Tg READ_BLOCK Addr

        Tg          = logical number of the card
        READ_BLOCK  = MIFARE Classic command code
        Addr        = address of the block to be read
    */

    if (!_pcd->issue_command(DATA_EXCHANGE, 1, READ_BLOCK, block_address)) {
        return false;
    }

    // We expect 16 bytes of response from the MIFARE Classic card
    if (!receive_command_response(contents, 16)) {
        return false;
    }

    return true;
};

bool MIFARE_Classic_PN532::write_block(unsigned char block_address, unsigned char* contents) {
    /*
        Write the 16 bytes of data specified in `contents` to the block at `block_address`

        Section 7.3.8 (PN532UM) describes format of the command:
        
        DATA_EXCHANGE Tg WRITE_BLOCK Addr Byte[0] ... Byte[15]
        
        Tg                      = logical number of the tag to be authenticated, we set to 1
        WRITE_BLOCK             = MIFARE Classic command code
        Addr                    = the address of the block to be authenticated, pass in `block_address`
        Byte[0] ... Byte[15]    = 16 bytes of data to be written to the block
    */

    unsigned char command_array[20];

    command_array[0] = DATA_EXCHANGE;
    command_array[1] = 1;
    command_array[2] = WRITE_BLOCK;
    command_array[3] = block_address;

    memcpy(command_array + 4, contents, 16);

    if (!_pcd->issue_command_from_array(command_array, 20)) {
        return false;
    }

    // Nothing received from the PICC is sent back to the host, simply check the Status byte
    return executed_successfully();
};