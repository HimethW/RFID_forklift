#ifndef PN5180_H
#define PN5180_H

#define SEND    0
#define RECEIVE 1

#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN5180_Registers.h"
#include "PN5180_Commands.h"

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

        template <typename PN5180_Command, typename... PN5180_Params>
        bool issue_command(PN5180_Command opcode, PN5180_Params... params) {
            // Put the opcode and parameters in an array
            uint8_t send_bytes[] = {opcode, params...};
            int length = sizeof(send_bytes) / sizeof(send_bytes[0]);

            // Send the command over SPI
            return transceive(SEND, send_bytes, length);
        };

        bool receive_response(uint8_t* response_buffer, int length);

    private:
        Pin _RST;
        Pin _BUSY;
        Pin _NSS;

        SPI_Master _spi;
};

#endif