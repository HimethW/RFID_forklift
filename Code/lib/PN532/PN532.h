#ifndef PN532_H
#define PN532_H

#define STATUS_READ             0x02
#define DATA_READ               0x03
#define DATA_WRITE              0x01

#define PREAMBLE                0x00
#define STARTCODE1              0x00 // Start of Packet Code
#define STARTCODE2              0xFF
#define TFI_HOST_TO_PN532       0xD4 // Target Frame Identifier for PN532
#define TFI_PN532_TO_HOST       0xD5 // Target Frame Identifier for Host
#define POSTAMBLE               0x00

#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN5180_Registers.h"
#include "PN5180_Commands.h"

const uint8_t ACK_FRAME[6] = {PREAMBLE, STARTCODE1, STARTCODE2, 0x00, 0xFF, POSTAMBLE};
const uint8_t NACK_FRAME[6] = {PREAMBLE, STARTCODE1, STARTCODE2, 0xFF, 0x00, POSTAMBLE};

class PN532 {
    public:
        PN532(Pin NSS, Pin MOSI, Pin MISO, Pin SCK);

        void initialize();

        bool send_bytes(uint8_t* bytes, int length);
        bool receive_bytes(uint8_t* buffer, int length);

        void make_normal_information_frame(uint8_t* target_frame, uint8_t TFI, uint8_t* bytes, uint8_t num_bytes);
        
        template <typename PN532_Command, typename... PN532_Params>
        bool issue_command(PN532_Command opcode, PN532_Params... params) {
            uint8_t send_bytes[] = {opcode, params...};
            int length = sizeof(send_bytes) / sizeof(send_bytes[0]);

            // Create the normal information frame with TFI
            uint8_t modified_spi_frame[length + 9];
            modified_spi_frame[0] = DATA_WRITE;
            make_normal_information_frame(modified_spi_frame + 1, TFI_HOST_TO_PN532, send_bytes, length);

            // Send the command over SPI
            return send_bytes(modified_spi_frame, length + 9);
        };

        bool receive_command_response(uint8_t* response_buffer, int length);

    private:
        Pin _NSS;
        SPI_Master _spi;
};

#endif