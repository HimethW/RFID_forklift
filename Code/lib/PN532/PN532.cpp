#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN5180_Registers.h"
#include "PN5180_Commands.h"
#include "PN532.h"

PN532::PN532(Pin NSS, Pin MOSI, Pin MISO, Pin SCK) : _NSS(NSS), _spi(MOSI, MISO, SCK) {
    _NSS.set_output(); // Initialize the NSS pin
    _NSS.assert(); // Keep NSS asserted
};

void PN532::initialize() {
    _spi.initialize(); // Initialize SPI
};

bool PN532::send_bytes(uint8_t* bytes, int length) {
    return _spi.send(bytes, length);
};

bool PN532::receive_bytes(uint8_t* buffer, int length) {
    return _spi.receive(buffer, length);
};

void PN532::make_normal_information_frame(uint8_t* target_frame, uint8_t TFI, uint8_t* bytes, uint8_t num_bytes) {
    // Create a frame with the given TFI and bytes
    uint8_t frame_size = num_bytes + 8; // Preamble, Start Codes, LEN, LCS, TFI, PD0, ... PDn, DCS, Postamble

    uint8_t DCS = PREAMBLE + STARTCODE1 + STARTCODE2 + TFI;

    target_frame[0] = PREAMBLE;
    target_frame[1] = STARTCODE1;
    target_frame[2] = STARTCODE2;
    target_frame[3] = num_bytes + 1;
    target_frame[4] = ~(num_bytes + 1) + 1;
    target_frame[3] = TFI;
    
    for (int i = 0; i < num_bytes; i++) {
        target_frame[4 + i] = bytes[i];
        DCS += bytes[i];
    }

    target_frame[num_bytes + 4] = ~DCS;
    target_frame[num_bytes + 5] = POSTAMBLE;
};

bool PN532::receive_command_response(uint8_t* response_buffer, int length) {
    // Send DATA_READ
    send_bytes((uint8_t[]){DATA_READ}, 1);

    // Receive the response
    if (!receive_bytes(response_buffer, length)) {
        return false; // Return false if receiving fails
    }

    return true;
};