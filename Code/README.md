## Microcontroller Program for Forklift-Mounted RFID Reader

The microcontroller code consists of the `main.cpp` file containing the main logic running on the device, and five important supplementary libraries, `Pins.h`, `Timer.h`, `SerialInterface.h`, `SPI.h`, and `PN532.h`.

### `Pins` Class

Abstracts away the register level manipulations required to control and communicate with the I/O pins of the ATMega328P. Each I/O pin on the ATMega328P belongs to one of three ports `B` through `D`, and assigned a number `0` through `7`, within that port.

#### Constructor
`Pin pin_name(unsigned char port, unsigned char pin)`

`port` can be any of `B`, `C`, or `D`, and `pin` is any integer from `0` to `7` (both inclusive). Initializes an object to control and communicate with the pin at that port and position.

e.g. `Pin my_pin(B, 7)` refers to pin `PB7` on the ATMega328P.

#### Methods
1. `set_output()`

    Configure the pin as an output pin.

2. `set_input()`

    Configure the pin as an input pin.

3. `assert()`

    Assert the pin, i.e., drive the pin high. Requires the pin to be configured as an output to work.

4. `deassert()`

    Deassert the pin, i.e., drive the pin low. Requires the pin to be configured as an output to work.

5. `toggle()`

    Toggle the pin from its previous state; high to low or low to high. Requires the pin to be configured as an output to work.

6. `state()`

    Read the current logic level at the pin. Requires the pin to be configured as an input to work. Returns `bool`: `true` if the pin is high and `false` otherwise.

7. `is_low()`

    Returns `bool`: `true` if the current logic level at the pin is low, and `false` otherwise. Requires the pin to be configured as an input to work.

8. `is_high()`

    Returns `bool`: `true` if the current logic level at the pin is high, and `false` otherwise. Requires the pin to be configured as an input to work.

### `Timer.h` Library

Uses `TIMER0` on the ATMega328P to generate delays.

#### Functions
1. `initialize_timer()`

    Enable `TIMER0` in the correct mode. Execute once before using any delays.

2. `blocking_delay(unsigned long duration, unsigned char unit)`

    Generate a blocking (program execution halted for the entire duration) delay of `duration` units of time, where `unit` specifies the unit of time. `unit` can be either `MILLISECONDS` or `MICROSECONDS`.

    e.g. `blocking_delay(1000, MILLISECONDS)` generates a blocking delay of 1000 milliseconds or 1 second.

### `SerialInterface` Class

Provides an interface to transmit and receive bytes over the hardware serial port on the ATMega328P. We assume that the CPU clock is 16 MHz.

#### Constructor
`SerialInterface serial_interface_name(unsigned long baud_rate)`

Initializes the hardware serial port to both transmit and receive over the specified `baud_rate`.

#### Methods
1. `send(unsigned char* send_bytes, int num_bytes)`

    Accepts a pointer `send_bytes` to an array of length `num_bytes`, and transmits each byte in the array over the hardware serial transmitter, one after the other, starting with `send_bytes[0]` and ending with `send_bytes[num_bytes - 1]`. Returns `bool`: `true` when all bytes are successfully transmitted.

2. `receive(unsigned char* receive_buffer, int num_bytes)`

    Accepts a pointer `receive_buffer` to an array of length `num_bytes`, and buffers the next `num_bytes` bytes read through the hardware serial receiver into it. The first byte read goes in `receive_buffer[0]` and the last one in `receive_buffer[num_bytes - 1]`. Returns `bool`: `true` when all bytes are successfully received.

### `SPI_Master` Class

Initializes the ATMega328P as an SPI Master and handles data transmission and reception.

#### Constructor
`SPI_Master spi_master_name()`

Define an `SPI_Master` object to access the hardware SPI port on the ATMega328P.

#### Methods
1. `initialize(unsigned char data_order)`

    Initialize the hardware SPI port to send and transmit in the specified `data_order`. `data_order` can be `MSB_FIRST` or `LSB_FIRST`.

2. `send_and_receive_byte(unsigned char send_byte, unsigned char* receive_byte)`

    Send the byte `send_byte` to the slave, and read the byte simultaneously transmitted by the slave into `receive_byte`. Returns `bool`: `true` when the operation is successfully completed.

3. `send(unsigned char* send_bytes, int num_bytes)`

    Accepts a pointer `send_bytes` to an array of length `num_bytes`, and transmits each byte in it over the MOSI pin. Returns `bool`: `true` once all bytes in the array are successfully transmitted.

4. `receive(unsigned char* receive_buffer, int num_bytes)`

    Accepts a pointer `receive_buffer` to an array of length `num_bytes`, and buffers `num_bytes` bytes in it over the MISO pin. Returns `bool`: `true` once `num_bytes` bytes are successfully received.

### `PN532` Class

Provides an interface to communicate with NXP's PN532 RFID chip.

#### Constructor
`PN532 my_pn532(Pin NSS)`

Define a `PN532` object to control the PN532, selected over the `NSS` `Pin`, i.e., `Pin` `NSS` on the ATMega328P must be connected to pin NSS on the PN532.

#### Methods
1. `initialize()`

    Initialize the hardware SPI port for communication with the PN532 and keep it deselected to begin with.

2. `send_bytes(unsigned char* bytes, int length)`

    Send the bytes in the array of length `length` pointed to by `bytes` to the PN532. Returns `bool`: `true` once all bytes are sent.

3. `receive_bytes(unsigned char* buffer, int length)`

    Buffer `length` bytes from the PN532 into the array pointed to by `buffer`. Returns `bool`: `true` when `length` bytes are received.

4. `write_frame(unsigned char* frame, int length)`

    Accept a pointer to an array `frame`, whose entries are the various bytes in a normal information frame, modify it as necessary, and send it to the PN532 over SPI (as defined in the PN532 User Manual).

5. `read_frame(unsigned char* frame_target, int length, bool start = false, bool conclude = false)`

    Buffer `length` bytes of a frame from the PN532 into the array pointed to by `frame_target`. `start` and `conclude` allow to read a frame portion by portion.
    
    If `start` is `true`, an initialization procedure to start reading a frame from the PN532 is performed. If `false`, it is assumed that the initialization was previously performed, and will not be performed; use if a `read_frame` was previously issued, but all bytes of the frame being sent were not read then.
    
    If `conclude` is `true`, a conclusion procedure to conclude reading the frame from the PN532 is performed. Set to `false` if not all of the bytes in the response frame are being buffered, and it is intended to buffer the remaining bytes in the response with future calls to `read_frame`.

    Returns `bool`: `true` if `length` bytes are read.

    e.g. Say the PN532 has a frame of length 20 ready to be read. Three calls to `read_frame` can be made as follows.
    
    `read_frame(buffer_1, 10, true, false)` performs the initialization procedure, and reads the first 10 bytes in the frame into `buffer_1`.
    
    `read_frame(buffer_2, 5, false, false)` reads the next 5 bytes of the frame response into `buffer_2`. The initialization procedure will not be performed.
    
    `read_frame(buffer_3, 5, false, true)` reads the last 5 bytes of the frame into `buffer_3` and performs the conclusion procedure, indicating we have completed reading the frame.

6. `make_normal_information_frame(unsigned char* target_frame, unsigned char TFI, unsigned char* bytes, unsigned char num_bytes)`

    Accept a pointer `bytes` to an array of length `num_bytes` containing the data bytes (`PD0` to `PDn` as per PN532's User Manual) to be included in a normal information frame, the relevant TFI, construct the frame with the frame header and trailer and place it in the array pointed to by `target_frame`.

    `TFI` can be either `TFI_PN532_TO_HOST` or `TFI_HOST_TO_PN532` and indicates the direction of data transfer.

    Two constants `FRAME_HEADER_SIZE` and `FRAME_TRAILER_SIZE` are defined in the library. It is required that `target_frame` have length `FRAME_HEADER_SIZE + num_bytes + FRAME_TRAILER_SIZE`.

7. `ready_to_respond()`

    Returns `bool`. Poll the PN532 for one second see if it has a frame ready to be read in that time. Returns `true` if a frame is detected, and `false` if a timeout occurs before.

8. `check_ack()`

    Returns `bool`. Read a frame from the PN532 and check if it is an `ACK` frame. Returns `true` if it is, and `false` if anything goes wrong.

9. `issue_command(PN532_Command opcode, PN532_Params... params)`

    Construct a normal information frame containing an instruction to the PN532 and send it over SPI. `opcode` must be an `unsigned char`, and possible values are in `PN532_Commands.h`. Follow `opcode` with a comma-separated list of all parameters/arguments required by it. Returns `bool`: `true` if the frame is successfully sent and acknowledged by the PN532.

10. `issue_command_from_array(unsigned char* command_array, int length)`

    Accept an array pointed to by `command_array` of length `length`, whose first entry is the command code (possible values in `PN532_Commands.h`) and following entries are arguments required by the command, and send it to the PN532 over SPI. Returns `bool`: `true` if the frame is successfully sent and acknowledged by the PN532.

11. `receive_command_response(unsigned char* response_buffer, int length, bool start = false, bool conclude = false)`

    See if the PN532 is ready to respond to a command previously issued, and buffer `length` bytes of the response frame into the array pointed to be `response_buffer`. `start` and `conclude` play the same roles as described in `read_frame`. Note that the frame header and trailer will not be stripped, and will also be placed in `response_buffer`; so it must at least have length `FRAME_HEADER_SIZE + 1 + FRAME_TRAILER_SIZE`. Returns `bool`: `true` once `length` bytes of the response are successfully read.

12. `SAMConfig()`

    Configure the PN532 to not use a SAM card. Returns `bool`: `true` if the command executed successfully.

13. `detect_card(unsigned char* card_number, unsigned char* card_data)`

    Scan the field for an ISO-14443 Type A compliant PICC, and if detected, place the logical number assigned to the card by the PN532 in `card_number`, and other details in the array pointed to by `card_data`.
    
    The length of `card_data` will depend on the number of UID bytes of the card and whether or not it is ISO-14443-4 compliant.

    In any case, the first four entries will always be respectively the MSB and LSB of the ATQA response, SAK, and the length of the UID in the number of bytes. The indices of these entries within this array will be `ATQA_MSB_IDX`, `ATQA_LSB_IDX`, `SAK_IDX`, and `UID_LEN_IDX` respectively.

    Hence, the following `card_data[UID_LEN_IDX]` bytes will contain the UID of the card. If the card is ISO-14443-4 compliant, `card[UID_LEN_IDX + card_data[UID_LEN_IDX]]` will contain the length of the `ATS` response, and the following `card[UID_LEN_IDX + card_data[UID_LEN_IDX]]` entries will contain its `ATS` response.

    Returns `bool`: `true` if a card is detected.

14. `get_mifare_classic_card()`

    Scan the field for MIFARE Classic Cards. Returns a pointer to an object of the `MIFARE_Classic_PN532` class if a card is found, containing its `UID`, and a null pointer otherwise.

### `MIFARE_Classic_PN532` Class

