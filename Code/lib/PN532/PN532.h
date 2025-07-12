#ifndef PN532_H
#define PN532_H

#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN532_Commands.h"

// SPI Shift Register Commands
#define STATUS_READ             0x02
#define DATA_READ               0x03
#define DATA_WRITE              0x01

// Frame Header and Trailer Bytes
#define PREAMBLE                0x00
#define STARTCODE1              0x00 // Start of Packet Code
#define STARTCODE2              0xFF
#define TFI_HOST_TO_PN532       0xD4 // Target Frame Identifier for PN532
#define TFI_PN532_TO_HOST       0xD5 // Target Frame Identifier for Host
#define POSTAMBLE               0x00

// Frame Array Indices
#define PREAMBLE_IDX            0
#define STARTCODE1_IDX          1
#define STARTCODE2_IDX          2
#define LEN_IDX                 3
#define LCS_IDX                 4
#define TFI_IDX                 5
#define OPCODE_IDX              6

#define ATQA_MSB_IDX            0
#define ATQA_LSB_IDX            1
#define SAK_IDX                 2
#define UID_LEN_IDX             3

// Frame, Frame Header, Frame Trailer Sizes
#define ACK_SIZE                6
#define NACK_SIZE               6
#define FRAME_HEADER_SIZE       6 // PREAMBLE ... TFI
#define FRAME_TRAILER_SIZE      2

const uint8_t ACK_FRAME[ACK_SIZE] = {PREAMBLE, STARTCODE1, STARTCODE2, 0x00, 0xFF, POSTAMBLE};
const uint8_t NACK_FRAME[NACK_SIZE] = {PREAMBLE, STARTCODE1, STARTCODE2, 0xFF, 0x00, POSTAMBLE};

class PN532 {
    public:
        PN532(Pin NSS, Pin MOSI, Pin MISO, Pin SCK);

        void initialize();

        bool send_bytes(uint8_t* bytes, int length);
        bool receive_bytes(uint8_t* buffer, int length);

        bool write_frame(uint8_t* frame, int length);
        bool read_frame(uint8_t* frame_target, int length, bool start = false, bool conclude = false);

        void make_normal_information_frame(uint8_t* target_frame, uint8_t TFI, uint8_t* bytes, uint8_t num_bytes);

        bool ready_to_respond();
        bool check_ack();
        
        template <typename PN532_Command, typename... PN532_Params>
        bool issue_command(PN532_Command opcode, PN532_Params... params) {
            /*
                Issue a command to the PN532, and check if it is ACK'ed
            */

            uint8_t data_bytes[] = {opcode, params...};
            int length = sizeof(data_bytes) / sizeof(data_bytes[0]);

            // Create a normal information frame by adding the frame header and trailer [Section 6.2.1.1 (PN532UM)]
            uint8_t normal_information_frame[FRAME_HEADER_SIZE + length + FRAME_TRAILER_SIZE];
            make_normal_information_frame(normal_information_frame, TFI_HOST_TO_PN532, data_bytes, length);

            if (!write_frame(normal_information_frame, FRAME_HEADER_SIZE + length + FRAME_TRAILER_SIZE)) {
                return false;
            }
            
            if (!ready_to_respond()) {
                return false;
            }

            return check_ack();
        };

        bool issue_command_from_array(uint8_t* command_array, int length);
        
        bool receive_command_response(uint8_t* response_buffer, int length, bool start = false, bool conclude = false);

        bool SAMConfig();

        bool detect_tag(uint8_t* tag_number, uint8_t* tag_data);

        bool authenticate_mifare_card_block(uint8_t tag_number, uint8_t* uid, uint8_t block_number, uint8_t* key);
        
        bool read_mifare_card_block(uint8_t tag_number, uint8_t block_number, uint8_t* response);
        bool write_mifare_card_block(uint8_t tag_number, uint8_t block_number, uint8_t* data);

    private:
        Pin _NSS;
        SPI_Master _spi;
};

#endif