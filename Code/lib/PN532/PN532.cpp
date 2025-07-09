#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN532_Commands.h"
#include "PN532.h"

PN532::PN532(Pin NSS, Pin MOSI, Pin MISO, Pin SCK) : _NSS(NSS), _spi(MOSI, MISO, SCK) {
    _NSS.set_output(); // Initialize the NSS pin
};

void PN532::initialize() {
    _spi.initialize(LSB_FIRST); // Initialize SPI

    _NSS.assert();
    _delay_ms(5);
};

bool PN532::send_bytes(uint8_t* bytes, int length) {
    return _spi.send(bytes, length);
};

bool PN532::receive_bytes(uint8_t* buffer, int length) {
    return _spi.receive(buffer, length);
};

void PN532::make_normal_information_frame(uint8_t* target_frame, uint8_t TFI, uint8_t* bytes, uint8_t num_bytes) {
    // Create a frame with the given TFI and bytes
    uint8_t DCS = PREAMBLE + STARTCODE1 + STARTCODE2 + TFI;

    target_frame[0] = PREAMBLE;
    target_frame[1] = STARTCODE1;
    target_frame[2] = STARTCODE2;
    target_frame[3] = num_bytes + 1; // 1 for TFI
    target_frame[4] = ~(num_bytes + 1) + 1;
    target_frame[5] = TFI;
    
    for (int i = 0; i < num_bytes; i++) {
        target_frame[6 + i] = bytes[i];
        DCS += bytes[i];
    }

    target_frame[num_bytes + 6] = ~DCS;
    target_frame[num_bytes + 7] = POSTAMBLE;
};

bool PN532::ready_to_respond() {
    int timeout = 1000; // Timeout in milliseconds
    bool ready = false;

    while (!(ready) && timeout > 0) {
        _NSS.deassert();

        _delay_ms(10);
        timeout -= 10; 

        uint8_t send_data[1] = {STATUS_READ};
        send_bytes(send_data, 1);

        uint8_t response_buffer[1];
        receive_bytes(response_buffer, 1);

        _NSS.assert();

        ready = (response_buffer[0] & 0x01);
    }

    return ready;
};

bool PN532::check_ack() {
    _NSS.deassert();
    _delay_ms(5);

    uint8_t send_data[1] = {DATA_READ};
    send_bytes(send_data, 1);

    uint8_t response_buffer[6];
    if (!receive_bytes(response_buffer, 6)) {
        _NSS.assert();
        return false; // Return false if receiving fails
    }

    _NSS.assert();

    // there are nicer ways of doing this.
    for (int i = 0; i < 6; i++) {
        if (response_buffer[i] != ACK_FRAME[i]) {
            return false; // If any byte does not match ACK_FRAME, return false
        }
    }

    return true;
};

bool PN532::receive_full_command_response(uint8_t* response_buffer, int length) {
    if (!ready_to_respond()) {
        return false;
    }

    _NSS.deassert();
    _delay_ms(5);

    // Send DATA_READ
    uint8_t send_data[1] = {DATA_READ};
    send_bytes(send_data, 1);

    // Receive the response
    if (!receive_bytes(response_buffer, length)) {
        _NSS.assert();
        return false; // Return false if receiving fails
    }

    _NSS.assert();

    return true;
};

bool PN532::initiate_receive_partial_command_response(uint8_t* response_buffer, int length) {
    if (!ready_to_respond()) {
        return false;
    }

    _NSS.deassert();
    _delay_ms(5);

    // Send DATA_READ
    uint8_t send_data[1] = {DATA_READ};
    send_bytes(send_data, 1);

    // Receive the response
    if (!receive_bytes(response_buffer, length)) {
        _NSS.assert();
        return false; // Return false if receiving fails
    }

    // Return without re-asserting NSS
    return true;
};

bool PN532::continue_receive_partial_command_response(uint8_t* response_buffer, int length) {
    if (!receive_bytes(response_buffer, length)) {
        _NSS.assert();
        return false; // Return false if receiving fails
    }

    return true;
};

void PN532::conclude_receive_partial_command_response() {
    _NSS.assert();
};

bool PN532::SAMConfig() {
    if (!issue_command(SAM_CONFIGURATION, 0x01, 0x14, 0x01)) { // directly copied from github. see why this must be done.
        return false;
    }

    uint8_t response[9];
    receive_full_command_response(response, 9);

    if ((response[5] == TFI_PN532_TO_HOST) && (response[6] == SAM_CONFIGURATION + 1)) {
        return true;
    } else {
        return false;
    }
};

bool PN532::detect_tag(uint8_t* tag_number, uint8_t* tag_data) {
    issue_command(LIST_PASSIVE_TARGETS, 0x01, 0x00);

    uint8_t response[8];

    // PREAMBLE STARTCODE1 STARTCODE2 LEN LCS TFI OPCODE+1 NbTg
    if (!initiate_receive_partial_command_response(response, 8)) {
        return false;
    }
    
    uint8_t number_of_tags = response[7];

    // Tg ATQA_MSB ATQA_LSB SAK UID_Length
    if (!continue_receive_partial_command_response(response, 5)) {
        return false;
    };

    *tag_number = response[0];
    
    // ATQA_MSB and ATQA_LSB in tag_data[0] and tag_data[1]
    tag_data[0] = response[1];
    tag_data[1] = response[2];
    
    // SAK in tag_data[2]
    uint8_t sak = response[3];
    tag_data[2] = sak;

    // UID Length in tag_data[3];
    int uid_length = response[4];
    tag_data[3] = uid_length;
    
    if (!continue_receive_partial_command_response(response, uid_length)) {
        return false;
    }

    // 4-byte UID goes in tag_data[4] ... tag_data[7]
    // 7-byte UID goes in tag_data[4] ... tag_data[10]
    // 10-byte UID goes in tag_data[4] ... tag_data[13]
    int i;
    for (i = 0; i < uid_length; i++) {
        tag_data[4 + i] = response[i];
    }

    if (sak & (1 << 6)) {
        // ISO 14443-4 Compliant

        if (!continue_receive_partial_command_response(response, 1)) {
            return false;
        }

        int ats_length = response[0];

        // ATS Length in tag_data[4 + i]
        tag_data[4 + i] = ats_length;

        if (!continue_receive_partial_command_response(response, ats_length)) {
            return false;
        }

        for (int j = 0; j < ats_length; j++) {
            tag_data[4 + i + 1 + j] = response[j];
        }
    }

    conclude_receive_partial_command_response();

    return true;
}