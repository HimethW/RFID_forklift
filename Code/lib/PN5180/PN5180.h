#ifndef PN5180_H
#define PN5180_H

#define SEND    0
#define RECEIVE 1

#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"

class PN5180 {
    public:
        PN5180(
            Pin RST,
            Pin BUSY,
            Pin NSS,
            Pin MOSI,
            Pin MISO,
            Pin SCK
        );

        void reset();
        void initialize();

        bool transceive(bool send_or_receive, uint8_t* data, int length);

    private:
        Pin _RST;
        Pin _BUSY;
        Pin _NSS;

        SPI_Master _spi;
};

#endif