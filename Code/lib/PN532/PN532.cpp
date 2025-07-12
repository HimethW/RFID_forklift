#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN532_Commands.h"
#include "PN532.h"

PN532::PN532(Pin NSS, Pin MOSI, Pin MISO, Pin SCK) : _NSS(NSS), _spi(MOSI, MISO, SCK) {
    _NSS.set_output();
};

void PN532::initialize() {
    _spi.initialize(LSB_FIRST);

    _NSS.assert(); // Keep the chip deactivated initially
    _delay_ms(5);
};

bool PN532::send_bytes(uint8_t* bytes, int length) {
    return _spi.send(bytes, length);
};

bool PN532::receive_bytes(uint8_t* buffer, int length) {
    return _spi.receive(buffer, length);
};

bool PN532::write_frame(uint8_t* frame, int length) {
    /*
        Send a DATA_WRITE byte first, then send the bytes contained in `frame`
    */
    
    // NSS assertion and deassertion as described in Section 8.3.5.5 (PN532DS)
    _NSS.deassert();
    _delay_ms(5);

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
    _delay_ms(5);

    return true;
};

bool PN532::read_frame(uint8_t* frame_target, int length, bool start, bool conclude) {
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
        _delay_ms(5);
        
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

void PN532::make_normal_information_frame(uint8_t* target_frame, uint8_t TFI, uint8_t* bytes, uint8_t num_bytes) {
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

    // uint8_t DCS = PREAMBLE + STARTCODE1 + STARTCODE2 + TFI;

    target_frame[PREAMBLE_IDX] = PREAMBLE;
    target_frame[STARTCODE1_IDX] = STARTCODE1;
    target_frame[STARTCODE2_IDX] = STARTCODE2;
    target_frame[LEN_IDX] = num_bytes + 1; // + 1 for TFI
    target_frame[LCS_IDX] = ~target_frame[LEN_IDX] + 1;
    target_frame[TFI_IDX] = TFI;
    
    uint8_t DCS = TFI;

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

        _delay_ms(10);
        timeout -= 10; 

        // Poll the Status byte and receive a byte of response [Section 6.2.5.1 (PN532UM)]
        _spi.send_and_receive_byte(STATUS_READ, nullptr);

        uint8_t response_buffer;
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

    uint8_t response[ACK_SIZE];
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

bool PN532::issue_command_from_array(uint8_t* command_array, int length) {
    /*
        Send `command_array` whose 0th entry is the command code and contains the relevant parameters in the following positions, and
        wait for it to be acknowledged
    */

    // Create a normal information frame by adding the frame header and trailer [Section 6.2.1.1 (PN532UM)]
    uint8_t normal_information_frame[FRAME_HEADER_SIZE + length + FRAME_TRAILER_SIZE];
    make_normal_information_frame(normal_information_frame, TFI_HOST_TO_PN532, command_array, length);

    if (!write_frame(normal_information_frame, FRAME_HEADER_SIZE + length + FRAME_TRAILER_SIZE)) {
        return false;
    }
    
    if (!ready_to_respond()) {
        return false;
    }

    return check_ack();
}

bool PN532::receive_command_response(uint8_t* response_buffer, int length, bool start, bool conclude) {
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
    */
    
    /*
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
    uint8_t response[FRAME_HEADER_SIZE + 1 + FRAME_TRAILER_SIZE];
    if (!receive_command_response(response, FRAME_HEADER_SIZE + 1 + FRAME_TRAILER_SIZE, true, true)) {
        return false;
    }

    return ((response[TFI_IDX] == TFI_PN532_TO_HOST) && (response[OPCODE_IDX] == SAM_CONFIGURATION + 1));
};

bool PN532::detect_tag(uint8_t* tag_number, uint8_t* tag_data) {
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

    uint8_t response[8];

    // Response begins OPCODE+1 NbTg ...
    if (!receive_command_response(response, FRAME_HEADER_SIZE + 2, true, false)) {
        return false;
    }
    
    uint8_t number_of_tags = response[7];

    // Response continues from above as ... Tg ATQA_MSB ATQA_LSB SAK UID_Length ...
    if (!receive_command_response(response, 5, false, false)) {
        return false;
    };

    *tag_number = response[0];
    
    tag_data[ATQA_MSB_IDX] = response[1];
    tag_data[ATQA_LSB_IDX] = response[2];
    
    uint8_t sak = response[3];
    tag_data[SAK_IDX] = sak;

    int uid_length = response[4];
    tag_data[UID_LEN_IDX] = uid_length;

    // As per Table 8, Section 6.4.3.4 (ISO-3)
    bool uid_complete = ~(sak & 0b100);
    bool iso14443_4_compliant = sak & 0b100000;

    // Next `uid_length` bytes of the response form the UID of the card
    // If the card is not ISO14443-4 compliant, the response ends after the UID; there will be no ATS
    if (!receive_command_response(response, uid_length, false, !iso14443_4_compliant)) {
        return false;
    }

    // 4-byte UID goes in `tag_data[4]` ... `tag_data[7]`
    // 7-byte UID goes in `tag_data[4]` ... `tag_data[10]`
    // 10-byte UID goes in `tag_data[4]` ... `tag_data[13]`
    int i;
    for (i = 0; i < uid_length; i++) {
        tag_data[4 + i] = response[i];
    }

    if (iso14443_4_compliant) { // There will be more bytes (ATS) to be read
        // Response continues as ... ATS_Length ...
        uint8_t ats_length;
        _spi.send_and_receive_byte(0x00, &ats_length);

        // Put ATS Length in `tag_data[4 + i]`
        tag_data[4 + i] = ats_length;

        // The next `ats_length` bytes form the ATS sent by the card; read them and conclude reading the frame
        if (!receive_command_response(response, ats_length, false, true)) {
            return false;
        }

        // Place the ATS bytes in the following indices
        for (int j = 0; j < ats_length; j++) {
            tag_data[4 + i + 1 + j] = response[j];
        }
    }

    return true;
};



bool PN532::authenticate_mifare_card_block(uint8_t tag_number, uint8_t* uid, uint8_t block_number, uint8_t* key) {
    /*
        Command format for MIFARE Cards is;

        DATA_EXCHANGE Tg Cmd Addr Data[0] ... Data[15]

        Tg                      = logical number of the selected target
        Cmd                     = MIFARE command code
        Addr                    = MIFARE block address
        Data[0] ... Data[15]    = 16 bytes of the relevant data to be sent
    */

    uint8_t command_array[];
    bool issued = issue_command(
        DATA_EXCHANGE, tag_number, 0x60, 0,
        key[0], key[1], key[2], key[3], key[4], key[5],
        uid[0], uid[1], uid[2], uid[3]
    );

    if (!issued) {
        return false;
    }

    uint8_t response[24];
    if (!receive_full_command_response(response, 24)) {
        return false;
    }

    if (response[7] == 0) {
        return true;
    } else {
        return false;
    }
};

bool PN532::read_mifare_card_block(uint8_t tag_number, uint8_t block_number, uint8_t* response) {
    if (!issue_command(DATA_EXCHANGE, tag_number, 0x30, block_number)) {
        return false;
    }

    if (!receive_full_command_response(response, 26)) {
        return false;
    }

    if (response[8] != 0) {
        return false;
    }

    return true;
};

bool PN532::write_mifare_card_block(uint8_t tag_number, uint8_t block_number, uint8_t* bytes) {
    bool issued = issue_command(
        DATA_EXCHANGE, tag_number, 0xA0, block_number,
        bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7],
        bytes[8], bytes[9], bytes[10], bytes[11], bytes[12], bytes[13], bytes[14], bytes[15]
    );

    if (!issued) {
        return false;
    }

    uint8_t response[26];
    if (!receive_full_command_response(response, 26)) {
        return false;
    }

    return true;
}