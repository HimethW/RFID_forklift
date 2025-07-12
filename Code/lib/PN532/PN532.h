/*
    PN532.h and MIFARE_Classic_Card.h

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Provides an interface to communicate with NXP's PN532 RFID module over SPI, and authenticate, read, and write blocks of MIFARE
    Classic Cards.

    The following sources were referenced.

    https://github.com/adafruit/Adafruit-PN532/blob/bf9f3f31961474fdbca23cb57d604c7f27269baf/Adafruit_PN532.h
    https://github.com/adafruit/Adafruit-PN532/blob/bf9f3f31961474fdbca23cb57d604c7f27269baf/Adafruit_PN532.cpp

    https://www.nxp.com/docs/en/user-guide/141520.pdf [PN532UM]
    https://www.nxp.com/docs/en/nxp/data-sheets/PN532_C1.pdf [PN532DS]
    http://www.emutag.com/iso/14443-3.pdf [ISO-3]

    https://www.nxp.com/docs/en/data-sheet/MF1S50YYX_V1.pdf
*/

#ifndef PN532_H
#define PN532_H

#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN532_Commands.h"
#include "MIFARE_Classic_Commands.h"

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
#define UID_START_IDX           4

// Frame, Frame Header, Frame Trailer Sizes
#define ACK_SIZE                6
#define NACK_SIZE               6
#define FRAME_HEADER_SIZE       6 // PREAMBLE ... TFI
#define FRAME_TRAILER_SIZE      2

const uint8_t ACK_FRAME[ACK_SIZE] = {PREAMBLE, STARTCODE1, STARTCODE2, 0x00, 0xFF, POSTAMBLE};
const uint8_t NACK_FRAME[NACK_SIZE] = {PREAMBLE, STARTCODE1, STARTCODE2, 0xFF, 0x00, POSTAMBLE};

class MIFARE_Classic_PN532;

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

            uint8_t data_bytes[] = {(uint8_t)(opcode), (uint8_t)(params)...};
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

        bool detect_card(uint8_t* card_number, uint8_t* card_data);
        
        MIFARE_Classic_PN532* get_mifare_classic_card();

    private:
        Pin _NSS;
        SPI_Master _spi;
};


class MIFARE_Classic_PN532 {
    public:
        MIFARE_Classic_PN532(PN532* pn532_pcd, uint8_t* uid, int uid_length);
        
        template <typename MIFARE_Classic_Command, typename MIFARE_Classic_Block, typename... MIFARE_Classic_Data>
        bool issue_command(MIFARE_Classic_Command mifare_command, MIFARE_Classic_Block block_address, MIFARE_Classic_Data... data) {
            /*
                Issue a MIFARE Classic Command to a selected MIFARE Classic Card using the DATA_EXCHANGE command of the PN532

                Format for DATA_EXCHANGE command with MIFARE Classic Cards on PN532 is

                DATA_EXCHANGE Tg Cmd Addr Data[0] ... Data[15]

                Tg                      = logical number of the selected target, we will set to 1
                Cmd                     = MIFARE Classic command code
                Addr                    = MIFARE Classic block address
                Data[0] ... Data[15]    = 16 bytes of the relevant data to be sent

                [Section 7.3.8 (PN532UM)]
            */

            uint8_t command_array[] = {DATA_EXCHANGE, 1, mifare_command, block_address, data...};
            int length = sizeof(command_array) / sizeof(command_array[0]);

            return _pcd->issue_command_from_array(command_array, length);
        };

        bool issue_command_from_array(uint8_t* command_array, int length);

        bool executed_successfully();
        bool receive_command_response(uint8_t* response_buffer, int length);

        bool authenticate_block(uint8_t authentication_type, uint8_t block_address, uint8_t* key);
        bool read_block(uint8_t block_address, uint8_t* contents);
        bool write_block(uint8_t block_address, uint8_t* contents);

    private:
        PN532* _pcd;
        
        uint8_t* _uid;
        int _uid_length;
};

#endif