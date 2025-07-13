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

#include "Pins.h"

#define MSB_FIRST   0
#define LSB_FIRST   1

// Relevant registers and bit positions [Section 31]
#define PRR0    *((volatile unsigned char*)(0x64))

#define SPCR    *((volatile unsigned char*)(0x4C))
#define SPSR    *((volatile unsigned char*)(0x4D))
#define SPDR    *((volatile unsigned char*)(0x4E))

#define PRSPI   2

#define SPR0    0
#define MSTR    4
#define DORD    5
#define SPE     6
#define SPIF    7

class SPI_Master {
    public:
        SPI_Master(Pin MOSI, Pin MISO, Pin SCK); // Assume SS is asserted and deasserted by the user
        
        void initialize(unsigned char data_order);

        bool send_and_receive_byte(unsigned char send_byte, unsigned char* receive_byte);

        bool send(unsigned char* send_bytes, int num_bytes);
        bool receive(unsigned char* receive_buffer, int num_bytes);
        
    private:
        Pin _MOSI, _SCK, _MISO;
};

#endif