#ifndef ISO14443_H
#define ISO14443_H

/*
    ## Short Frame Structure
    E   b7 b6 b5 b4 b3 b2 b1   S

    E               -   End of communication
    b7, ..., b1     -   Data bits
    S               -   Start of communication

    ## ATQA Structure
    b16 b15 b14 b13 b12 b11 b10 b09 | b08 b07 b06 b05 b04 b03 b02 b01

    b16, ..., b13   - RFU
    b12, ..., b09   - Proprietary
    b08, b07        - UID Size
    b06             - RFU
    b05, ..., b01   - Bit Frame Anticollision

    UID Size: b08 b07
    00 - SINGLE
    01 - DOUBLE
    10 - TRIPLE
    11 - RFU

    Bit Frame Anticollision: b05 ... b01
    One and only one of these bits will be 1, the rest will be 0

    ## Anticollision Frame Structure
    S   b7  b6  b5  b4  b3  b2  b1   E

*/

/*
    Expects two callbacks; send_callback and receive_callback

    send_callback(uint8_t valid_bits_last_byte, uint8_t byte1, uint8_t byte2, ...)
    Sends the specified bytes through the RF field.

    - valid_bits_last_byte  :   Number of valid bits in the last byte to be sent
    - byte1, byte2, ...     :   Bytes to be sent

    receive_callback(uint8_t* buffer, uint8_t length)
    Reads bytes transmitted back through the RF field.

    - buffer                :   Pointer to the buffer where received data will be stored
    - length                :   Number of bytes to be received
*/

#define REQA_FRAME      0x26
#define WUPA_FRAME      0x52

#define SEL_CL1         0x93
#define SEL_CL2         0x95
#define SEL_CL3         0x97

template <typename send_callback, typename receive_callback, typename collision_callback>
class ISO14443A_PCD {
    public:
        ISO14443A_PCD(send_callback send_cb, receive_callback receive_cb, collision_callback collision_cb)
            : _send_callback(send_cb), _receive_callback(receive_cb), _collision_callback(collision_cb) {
                ;
        };

        void send_REQA() {
            _send_callback(7, REQA_FRAME);  // Send the REQA command
        };

        void send_WUPA() {
            _send_callback(7, WUPA_FRAME);  // Send the WUPA command
        };

        void receive_ATQA(uint8_t* atqa_buffer) {
            _receive_callback(atqa_buffer, 2);  // Receive 2 bytes for ATQA
        };

        template <typename... bytes>
        void send_AC(uint8_t CLn, uint8_t NVB, bytes... uncollided_bytes) {
            if (NVB == 0x20) {
                // No bytes have been read, we ask to send all UID bytes
                _send_callback(0, CLn, NVB);  // Send CLn and NVB
            } else {
                uint8_t num_uncollided_bits = (NVB - 0x20) % 8;
                _send_callback(num_uncollided_bits, CLn, NVB, uncollided_bytes...);
            }
        }

        void receive_AC(uint8_t* receive_buffer, uint8_t* uncollided_bytes) {
            _receive_callback(receive_buffer, 5);  // Receive UID bytes, we must read 5 bytes
            uint8_t first_collision_pos = _collision_callback();
            // Store into uncollided bytes the bytes that were not collided reading from receive_buffer
        };



    private:
        send_callback _send_callback;
        receive_callback _receive_callback;
        collision_callback _collision_callback;
};

#endif