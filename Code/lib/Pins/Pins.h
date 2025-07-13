/*
    Pins.h

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

#ifndef PINS_H
#define PINS_H

#define B   0x03
#define C   0x06
#define D   0x09

class Pin {
    public:
        Pin(unsigned char port, unsigned char pin);
        
        void set_output();
        void set_input();

        void assert();
        void deassert();
        void toggle();

        bool state();
        bool is_low();
        bool is_high();

    private:
        volatile unsigned char* _port_data_register;
        volatile unsigned char* _port_data_direction_register;
        volatile unsigned char* _port_input_pin_address_register;
        unsigned char _pin_position;
};

#endif