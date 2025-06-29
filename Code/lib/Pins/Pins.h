#ifndef PINS_H
#define PINS_H

#include "avr/io.h"

#define B 0
#define C 1
#define D 2
#define E 3

class Pin {
    public:
        Pin(uint8_t port, uint8_t pin);
        
        void set_output();
        void set_input();

        void assert();
        void deassert();
        void toggle();

        bool state();
        bool is_low();
        bool is_high();

    private:
        uint8_t _port;
        uint8_t _pin;
};

#endif