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

  pn532.issue_command(GET_FIRMWARE_VERSION);
  Serial.println("GET FIRMWARE COMMAND ISSUED");

  uint8_t response[13];
  if (pn532.receive_full_command_response(response, 13)) {
    Serial.println("RESPONSE IS ");
    for (int i = 0; i < 13; i++) {
      Serial.print("0x");
      Serial.print(response[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  
}

void loop() {
  pn532.issue_command(LIST_PASSIVE_TARGETS, 0x01, 0x00);
  Serial.println("ISSUED COMMAND TO READ");

  uint8_t response[20];
  bool x = pn532.initiate_receive_partial_command_response(response, 8);
  if (x) {
    uint8_t number_of_tags = response[7];
    for (int i = 1; i <= number_of_tags; i++) {
      Serial.print("TAG ");
      Serial.println(i);
      bool y = pn532.continue_receive_partial_command_response(response, 5);
      if (y) {
        Serial.print("ATQA: ");
        Serial.print(response[1], BIN);
        Serial.print(" ");
        Serial.println(response[2], BIN);

        uint8_t sak = response[3];
        Serial.print("SAK: ");
        Serial.println(sak, BIN);

        int uid_length = response[4];
        Serial.print("UID Length: ");
        Serial.println(uid_length);

        bool z = pn532.continue_receive_partial_command_response(response, uid_length);
        if (z) {
          Serial.print("UID: ");
          for (int j = 0; j < uid_length; j++) {
            Serial.print(response[j], HEX);
            Serial.print(", ");
          }
          Serial.println();

          if (sak & (1 << 6)) {
            Serial.println("ISO 14443-4 Compliant");

            bool a = pn532.continue_receive_partial_command_response(response, 1);
            if (a) {
              int ats_length = response[0];
              Serial.print("ATS Length: ");
              Serial.println(ats_length);

              bool b = pn532.continue_receive_partial_command_response(response, ats_length);
              if (b) {
                Serial.print("ATS: ");
                for (int k = 0; k < uid_length; k++) {
                  Serial.print(response[k], HEX);
                  Serial.print(", ");
                }
                Serial.println();

                pn532.conclude_receive_partial_command_response();
              }
            }
          } else {
            Serial.println("NOT ISO14443-4 Compliant");
          }
        }
      }
    }
  } else {
    Serial.println("NO RESP");
  }

  delay(1500);
}