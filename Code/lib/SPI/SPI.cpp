#include "avr/io.h"
#include "Pins.h"
#include "SPI.h"

SPI_Master::SPI_Master(Pin MOSI, Pin MISO, Pin SCK) : _MOSI(MOSI), _MISO(MISO), _SCK(SCK) {
    ;
};

void SPI_Master::initialize(uint8_t data_order) {
    // Set up the SPI pins
    _MOSI.set_output();
    _SCK.set_output();
    _MISO.set_input();
    
    // Ensure the SPI is enabled in the Power Reduction Register (PRR)
    PRR &= ~(1 << PRSPI);

    // Set up the SPI Control Register (SPCR)
    // Enable SPI, setup as master, and set clock rate to 1/16 CPU clock
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (data_order << DORD);
};

bool SPI_Master::_send_and_receive_byte(uint8_t send_byte, uint8_t* receive_byte) {
    // Write the data to be sent into the SPI Data Register (SPDR)
    SPDR = send_byte;

    // Wait for transmission to complete
    while (!(SPSR & (1 << SPIF))) {
        // Do nothing, this is blocking.
    }

    // SPDR now contains a byte received from the slave
    *receive_byte = SPDR;

    return true;  // Return true if the operation was successful
};

bool SPI_Master::send(uint8_t* send_bytes, int num_bytes) {
    // Use send_and_receive_byte on each byte in send_bytes
    for (int i = 0; i < num_bytes; i++) {
        uint8_t dummy_receive_byte;
        if (!_send_and_receive_byte(send_bytes[i], &dummy_receive_byte)) {
            return false; // If any byte is not sent correctly, return false
        };
    }
    return true; // Return true if all bytes were sent successfully
};

bool SPI_Master::receive(uint8_t* receive_buffer, int num_bytes) {
    // Use send_and_receive_byte to receive a known number of bytes
    for (int i = 0; i < num_bytes; i++) {
        uint8_t receive_byte;
        if (!_send_and_receive_byte(0x00, &receive_byte)) { // Send a dummy byte to receive data
            return false; // If any byte is not received correctly, return false
        } else {
            receive_buffer[i] = receive_byte; // Store the received byte in the buffer
        }
    }
    return true; // Return true if all bytes were received successfully
};