#include <Arduino.h>
#include <Pins.h>
#include <SPI.h>
#include <PN5180.h>

#include <avr/io.h>
#include <avr/delay.h>

Pin RST(B, 4);  // Digital Pin 8
Pin BUSY(B, 5); // Digital Pin 9
Pin HW_MOSI(B, 2);
Pin HW_MISO(B, 3);
Pin HW_SCK(B, 1);
Pin NSS(B, 6);  // Digital Pin 10

Pin LED(C, 7);  // L LED

PN5180 nfc(RST, BUSY, NSS, HW_MOSI, HW_MISO, HW_SCK);

void setup() {
  Serial.begin(115200);

  delay(2000);
  Serial.println("HELLO");
  delay(1000);

  LED.set_output();

  Serial.println("INITIALIZING");
  nfc.initialize();
  Serial.println("INITIALIZED");

  nfc.issue_command(READ_EEPROM, 0x10, 0x02);
  Serial.println("send complete");

  uint8_t receive_buffer[2];
  nfc.receive_response(receive_buffer, 2);  // Receive 2 bytes
  Serial.println("receive complete");
  
  Serial.print("I RECEIVED ");
  Serial.println(receive_buffer[1]);
  Serial.println("!!!!!!!!!!!!!!!!");
  
  if (receive_buffer[1] == 4) {
    LED.assert();  // Turn on LED
    _delay_ms(1000);
    LED.deassert();  // Turn off LED
    _delay_ms(1000);
  }

  _delay_ms(1000);
}

void loop() {
  if (nfc.issue_command(LOAD_RF_CONFIG, 0x00, 0x80)) {
    Serial.println("LOAD_RF_CONFIG command issued");
  }

  if (nfc.issue_command(RF_ON, 0x00)) {
    Serial.println("RF_ON command issued");
  }

  _delay_ms(500);

  if (nfc.issue_command(WRITE_REGISTER_AND, REG_CRC_RX_CONFIG, 0xFE, 0xFF, 0xFF, 0xFF)) {
    Serial.println("WRITE_REGISTER_AND command issued, CRC RX turned off");
  }

  if (nfc.issue_command(WRITE_REGISTER_AND, REG_CRC_TX_CONFIG, 0xFE, 0xFF, 0xFF, 0xFF)) {
    Serial.println("WRITE_REGISTER_AND command issued, CRC TX turned off");
  }
  
  if (nfc.issue_command(WRITE_REGISTER, REG_IRQ_CLEAR, 0xFF, 0xFF, 0x0F, 0x00)) {
    Serial.println("IRQ_STATUS cleared");
  }

  if (nfc.issue_command(WRITE_REGISTER_AND, REG_SYSTEM_CONFIG, 0xF8, 0xFF, 0xFF, 0xFF)) {
    Serial.println("IDLE state");
  }

  if (nfc.issue_command(WRITE_REGISTER_OR, REG_SYSTEM_CONFIG, 0x03, 0x00, 0x00, 0x00)) {
    Serial.println("Transceive state");
  }

  if (nfc.issue_command(SEND_DATA, 0x07, 0x26)) {
    Serial.println("REQA SENT");
  }
  
  _delay_ms(1);

  if (nfc.issue_command(READ_DATA, 0x00)) {
    Serial.println("READ_DATA command issued");
  }

  uint8_t response_buffer[2];
  if (nfc.receive_response(response_buffer, 2)) {
    Serial.println("Response received");
    Serial.print("ATQA: ");
    for (int i = 0; i < 2; i++) {
      Serial.print(response_buffer[i], BIN);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("Failed to receive response");
  }

  _delay_ms(2000);
}