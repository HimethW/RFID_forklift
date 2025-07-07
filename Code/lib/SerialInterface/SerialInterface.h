#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include "avr/io.h"

class SerialInterface {
    public:
        SerialInterface(unsigned long system_oscillator_frequency, unsigned long baud_rate);
        bool send(uint8_t* bytes, int length);
        bool receive(uint8_t* bytes, int length);

    private:
};

#endif