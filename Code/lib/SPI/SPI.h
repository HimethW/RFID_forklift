/*
    SPI.h

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Simplifies the usage of the ATMega328P's hardware SPI pins.

    The following sources were referenced.

    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*/

#ifndef SPI_H
#define SPI_H

#include "avr/io.h"
#include "Pins.h"

#define MSB_FIRST   0
#define LSB_FIRST   1

class SPI_Master {
    public:
        SPI_Master(Pin MOSI, Pin MISO, Pin SCK); // Assume SS is asserted and deasserted by the user
        
        void initialize(uint8_t data_order);

        bool send_and_receive_byte(uint8_t send_byte, uint8_t* receive_byte);

        bool send(uint8_t* send_bytes, int num_bytes);
        bool receive(uint8_t* receive_buffer, int num_bytes);
        
    private:
        Pin _MOSI, _SCK, _MISO;
};

#endif