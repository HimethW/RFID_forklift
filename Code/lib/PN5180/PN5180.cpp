#include "avr/io.h"
#include "avr/delay.h"
#include "Pins.h"
#include "SPI.h"
#include "PN5180_Registers.h"
#include "PN5180_Commands.h"
#include "PN5180.h"

PN5180::PN5180(
    Pin RST,
    Pin BUSY,
    Pin NSS,
    Pin MOSI,
    Pin MISO,
    Pin SCK
) : _RST(RST), _BUSY(BUSY), _NSS(NSS), _spi(MOSI, MISO, SCK)  { // Create SPI in the constructor
    // Initialize other pins
    _NSS.set_output();
    _RST.set_output();
    _BUSY.set_input();

    // Keep NSS asserted
    _NSS.assert();
};

void PN5180::reset() {
    _RST.deassert(); // Set RST to low to reset
    _delay_ms(100);  // Wait 100 ms
    _RST.assert(); // Set RST high to release from resets
    _delay_ms(100);  // Wait another 100 mss
};

void PN5180::initialize() {
    // Initialize SPI
    _spi.initialize();

    // Reset the PN5180
    reset();
};

bool PN5180::transceive(bool send_or_receive, uint8_t* data, int length) {
    // Follows the procedure described in the datasheet to exchange data

    // 0. Wait for BUSY to go low
    while (_BUSY.is_high()) { ; }

    // 1. Deassert NSS
    _NSS.deassert();

    // 2. Send/Receive data
    if (send_or_receive == SEND) {
        if (!_spi.send(data, length)) {
            return false;  // Return false if sending fails
        }
    } else {
        if (!_spi.receive(data, length)) {
            return false;  // Return false if receiving fails
        }
    }

    // 3. Wait for BUSY to go high
    while (_BUSY.is_low()) { ; }

    // 4. Assert NSS
    _NSS.assert();

    // 5. Wait for BUSY to go low again
    while (_BUSY.is_high()) { ; }

    return true;  // Return true if everything was successful
};

bool PN5180::issue_command(uint8_t* command_bytes, uint8_t num_command_bytes) {
    return transceive(SEND, command_bytes, num_command_bytes);
};

bool PN5180::receive_command_response(uint8_t* response_buffer, int length) {
    // Receive the response from the PN5180
    return transceive(RECEIVE, response_buffer, length);
};

bool PN5180::transmit_rf(uint8_t valid_bits_last_byte, uint8_t num_bytes, uint8_t* bytes) {
    // Clear the TX_RFON_IRQ_STAT bit in the IRQ_STATUS register
    issue_command(WRITE_REGISTER, REG_IRQ_CLEAR, 0xFF, 0xFF, 0xFF, 0xFF);

    // registers are read
    // b7 b6 b5 b4 b3 b2 b1 b0 | b15 b14 b13 b12 b11 b10 b9 b8 etc

    // Turn on the RF field
    issue_command(RF_ON, 0x00);
    _delay_ms(1);  // Wait for a short time to ensure the RF field is stable
    
    // See that the field is on by reading IRQ_STATUS
    issue_command(READ_REGISTER, REG_IRQ_STATUS);
    uint8_t irq_status[4];
    receive_command_response(irq_status, 4);
    
    if (!(irq_status[1] & 0x02)) {  // Check if the RF field is on
        Serial.println("RF field is not on, cannot transmit RF data.");
    } else {
        Serial.println("RF field is on, proceeding to transmit RF data.");
    }

    // We will first move to the IDLE state and enter the TRANSCEIVE state
    issue_command(WRITE_REGISTER_AND, REG_SYSTEM_CONFIG, 0xF8, 0xFF, 0xFF, 0xFF); // Set 3 LSBs to 000 for IDLE
    _delay_ms(1);
    issue_command(WRITE_REGISTER_OR, REG_SYSTEM_CONFIG, 0x03, 0x00, 0x00, 0x00); // Set 3 LSBs to 100 for TRANSCEIVE

    // Read RF_STATUS register, which stores the state of the transceive machine
    issue_command(READ_REGISTER, REG_RF_STATUS);
    uint8_t rf_status[4];
    receive_command_response(rf_status, 4);  // Receive the 4 bytes of RF_STATUS
    
    // Check if the state in bits 24:26 is WAIT_TRANSMIT (0x02)
    if ((rf_status[3] & 0x07) != 0x01) {
        Serial.println("We are not in the WAIT_TRANSMIT state, cannot transmit RF data.");
        //return false;
    } else {
        Serial.println("We are in the WAIT_TRANSMIT state, proceeding to transmit RF data.");
    }

    // Send the data
    uint8_t command_bytes[num_bytes + 2];
    command_bytes[0] = SEND_DATA;
    command_bytes[1] = valid_bits_last_byte;
    for (int i = 0; i < num_bytes; i++) {
        command_bytes[2 + i] = bytes[i];
    }
    issue_command(command_bytes, num_bytes + 2);
    Serial.println("DATA SENT");

    _delay_ms(1000);

    // Read RF_STATUS register, which stores the state of the transceive machine
    issue_command(READ_REGISTER, REG_RF_STATUS);
    uint8_t rf_statuss[4];
    receive_command_response(rf_statuss, 4);  // Receive the 4 bytes of RF_STATUS
    Serial.print("RF_STATUS: ");
    for (int i = 0; i < 4; i++) {
        Serial.print(rf_statuss[i], BIN);
        Serial.print(" ");
    };
    Serial.println();
    
    // Check if the state in bits 24:26 is WAIT_TRANSMIT (0x02)
    Serial.println("WAiting for WAIT_RECEIVE state");
    while ((rf_statuss[3] & 0x07) != 0x03) {
        issue_command(READ_REGISTER, REG_RF_STATUS);
        receive_command_response(rf_statuss, 4);  // Receive the 4 bytes of RF_STATUS
        Serial.print(rf_statuss[0], BIN);
        Serial.print(" ");
        Serial.print(rf_statuss[1], BIN);
        Serial.print(" ");
        Serial.print(rf_statuss[2], BIN);
        Serial.print(" ");
        Serial.println(rf_statuss[3], BIN);
        _delay_ms(100);
        //return false;
    }
    
    Serial.println("We are in the WAIT_RECEIVE state.");

    // Clear the TX_RFOFF_IRQ_STAT bit in the IRQ_STATUS register
    issue_command(WRITE_REGISTER, REG_IRQ_CLEAR, 0xFF, 0xFF, 0xFF, 0xFF);

    // Turn off the RF field
    Serial.println("turning off RF field");
    issue_command(RF_OFF, 0x00);
    _delay_ms(1);  // Wait for a short time to ensure the RF field is stable
    
    // See that the field is on by reading IRQ_STATUS
    Serial.println("checking RF field status after turning off");
    issue_command(READ_REGISTER, REG_IRQ_STATUS);
    uint8_t irq_statuss[4];
    receive_command_response(irq_statuss, 4);
    Serial.println("Received IRQ_STATUS:");
    Serial.print("IRQ_STATUS: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(irq_statuss[i], BIN);
      Serial.print(" ");
    }
    Serial.println();
    // or consider putting in a loop
    if (!(irq_statuss[1] & 0x01)) {  // Check if the RF field is off
        Serial.println("RF field is not off.");
    } else {
        Serial.println("RF field is off, transmission complete.");
    }

    return true;
};