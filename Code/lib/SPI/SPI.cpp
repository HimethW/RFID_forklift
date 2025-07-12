/*
    SPI.cpp

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Simplifies the usage of the ATMega328P's hardware SPI pins.

    The following sources were referenced.

    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*/

#include "avr/io.h"
#include "Pins.h"
#include "SPI.h"

SPI_Master::SPI_Master(Pin MOSI, Pin MISO, Pin SCK) : _MOSI(MOSI), _MISO(MISO), _SCK(SCK) {
    ;
};

void SPI_Master::initialize(uint8_t data_order) {
    // Follows the example in Section 18.2

    // Set up the SPI pins
    _MOSI.set_output();
    _SCK.set_output();
    _MISO.set_input();
    
    // Ensure the SPI is enabled in the Power Reduction Register (PRR) [Section 9.11.3]
    PRR &= ~(1 << PRSPI);

    // Set up the SPI Control Register (SPCR) [Section 18.5.1]
    // Enable SPI, setup as master, and set clock rate to 1/16 CPU clock
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (data_order << DORD);
};

bool SPI_Master::send_and_receive_byte(uint8_t send_byte, uint8_t* receive_byte) {
    // Follows the example in Section 18.2 and Section 18.5.2

    // Write the data to be sent into the SPI Data Register (SPDR)
    SPDR = send_byte;

    // Wait for transmission to complete
    while (!(SPSR & (1 << SPIF))) {
        // Do nothing, this is blocking. Consider timing out.
    }

    // SPDR now contains a byte received from the slave, save it in receive_byte if it is not a null pointer
    if (receive_byte) {
        *receive_byte = SPDR;
    }

    return true;
};

bool SPI_Master::send(uint8_t* send_bytes, int num_bytes) {
    // Use send_and_receive_byte on each byte in send_bytes
    for (int i = 0; i < num_bytes; i++) {
        if (!send_and_receive_byte(send_bytes[i], nullptr)) {
            return false;
        };
    }
    return true;
};

bool SPI_Master::receive(uint8_t* receive_buffer, int num_bytes) {
    // Use send_and_receive_byte to receive a known number of bytes
    for (int i = 0; i < num_bytes; i++) {
        uint8_t receive_byte;
        if (!send_and_receive_byte(0x00, &receive_byte)) { // Send a dummy byte to receive data
            return false;
        } else {
            receive_buffer[i] = receive_byte;
        }
    }
    return true;
};