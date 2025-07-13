/*
    Pins.cpp

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Abstracts away the direct register manipulation involved in controlling the I/O pins of the ATMega328P.

    The following sources were referenced.

    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
    [Section 10.4], [Section 31]
*/

#include "Pins.h"

Pin::Pin(unsigned char port, unsigned char pin) {
    _port_input_pin_address_register = (volatile unsigned char*)(port + 0x20);
    _port_data_direction_register = (volatile unsigned char*)(port + 0x21);
    _port_data_register = (volatile unsigned char*)(port + 0x22);

    _pin_position = pin;
};

void Pin::set_output() {
    *_port_data_direction_register |= (1 << _pin_position);
}

void Pin::set_input() {
    *_port_data_direction_register &= ~(1 << _pin_position);
};

void Pin::assert() {
    *_port_data_register |= (1 << _pin_position);
};

void Pin::deassert() {
    *_port_data_register &= ~(1 << _pin_position);
};

void Pin::toggle() {
    *_port_data_register ^= (1 << _pin_position);
};

bool Pin::state() {
    return (*_port_input_pin_address_register & (1 << _pin_position));
};

bool Pin::is_low() {
    return !state();
};

bool Pin::is_high() {
    return state();
};