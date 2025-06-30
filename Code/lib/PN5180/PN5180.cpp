#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN5180_Registers.h"
#include "PN5180_Commands.h"
#include "PN5180.h"

PN5180::PN5180(
    Pin RST,
    Pin BUSY,
    Pin NSS,
    Pin MOSI,
    Pin MISO,
    Pin SCK
) : _RST(RST), _BUSY(BUSY), _NSS(NSS), _spi(MOSI, MISO, SCK)  { // Create SPI in the constructor
    // Initialize other pins
    _NSS.set_output();
    _RST.set_output();
    _BUSY.set_input();

    // Keep NSS asserted
    _NSS.assert();
};

void PN5180::reset() {
    _RST.deassert(); // Set RST to low to reset
    _delay_ms(100);  // Wait 100 ms
    _RST.assert(); // Set RST high to release from resets
    _delay_ms(100);  // Wait another 100 mss
};

void PN5180::initialize() {
    // Initialize SPI
    _spi.initialize();

    // Reset the PN5180
    reset();
};

bool PN5180::transceive(bool send_or_receive, uint8_t* data, int length) {
    // Follows the procedure described in the datasheet to exchange data

    // 0. Wait for BUSY to go low
    while (_BUSY.is_high()) { ; }

    // 1. Deassert NSS
    _NSS.deassert();

    // 2. Send/Receive data
    if (send_or_receive == SEND) {
        if (!_spi.send(data, length)) {
            return false;  // Return false if sending fails
        }
    } else {
        if (!_spi.receive(data, length)) {
            return false;  // Return false if receiving fails
        }
    }

    // 3. Wait for BUSY to go high
    while (_BUSY.is_low()) { ; }

    // 4. Assert NSS
    _NSS.assert();

    // 5. Wait for BUSY to go low again
    while (_BUSY.is_high()) { ; }

    return true;  // Return true if everything was successful
};

bool PN5180::receive_response(uint8_t* response_buffer, int length) {
    // Receive the response from the PN5180
    return transceive(RECEIVE, response_buffer, length);
};