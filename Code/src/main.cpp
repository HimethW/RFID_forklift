#include <Arduino.h>
#include <Pins.h>
#include <SPI.h>
#include <PN532.h>

#include <avr/io.h>
#include <avr/delay.h>

Pin HW_MOSI(B, 2);
Pin HW_MISO(B, 3);
Pin HW_SCK(B, 1);
Pin NSS(B, 0);

PN532 pn532(NSS, HW_MOSI, HW_MISO, HW_SCK);

void setup() {
  Serial.begin(115200);

  delay(2000);
  Serial.println("HELLO");
  delay(1000);

  pn532.initialize();
  delay(100);
  Serial.println("initialized");

  delay(100);

  if (pn532.SAMConfig()) {
    Serial.println("SAM CONFIG DONE");
  }

  delay(100);
}

void loop() {
  pn532.issue_command(GET_FIRMWARE_VERSION);
  Serial.println("GET_FIRMWARE_VERSION command issued");
  
  delay(100);

  uint8_t response[13];
  bool got = pn532.receive_command_response(response, 13);

  if (got) {
    Serial.println("Response received:");
  } else {
    Serial.println("Failed to receive response. But here");
  }

  for (int i = 0; i < 13; i++) {
    Serial.print("0x");
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  delay(1500);
}