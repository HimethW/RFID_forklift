#ifndef SPI_H
#define SPI_H

#include "avr/io.h"
#include "Pins.h"

class SPI_Master {
    public:
        SPI_Master(
            Pin MOSI,
            Pin MISO,
            Pin SCK
        );
        // Assume SS is asserted and deasserted by the user
        
        void initialize();

        bool send(uint8_t* send_bytes, int num_bytes);
        bool receive(uint8_t* receive_buffer, int num_bytes);
        
    private:
        Pin _MOSI;
        Pin _SCK;
        Pin _MISO;

        bool _send_and_receive_byte(uint8_t send_byte, uint8_t* receive_byte);
};

#endif