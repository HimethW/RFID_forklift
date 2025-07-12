/*
    SerialInterface.h

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Simplfies the usage of the ATMega328P's hardware USART.

    The following sources were referenced.

    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*/

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include "avr/io.h"

class SerialInterface {
    public:
        SerialInterface(unsigned long system_oscillator_frequency, unsigned long baud_rate);
        bool send(uint8_t* bytes, int length);
        bool receive(uint8_t* bytes, int length);
};

#endif