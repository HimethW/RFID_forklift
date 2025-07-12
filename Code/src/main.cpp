#include <Arduino.h>
#include <Pins.h>
#include <SerialInterface.h>
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
  Serial.begin(9600);
  delay(1000);
  Serial.println("HI");

  pn532.initialize();
  _delay_ms(100);

  pn532.SAMConfig();
}

void loop() {
  MIFARE_Classic_PN532* card;
  card = pn532.get_mifare_classic_card();
  if (card) {
    Serial.println("FOUND");
    uint8_t key[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    bool ok = card->authenticate_block(AUTHENTICATE_KEY_A, 0x02, key);
    if (ok) {
      Serial.println("AUTHED");
      
      uint8_t contents[16];
      if (card->read_block(0x02, contents)) {
        Serial.println("READ");
        for (int i = 0; i < 16; i++) {
          Serial.print(contents[i], HEX);
          Serial.print(", ");
        }
        Serial.println();
      } else {
        Serial.println("CUDNT READ");
      }

      delay(2000);

      Serial.println("GONNA WRITE");
      
      uint8_t newcont[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
      
      if (card->write_block(0x02, newcont)) {
        Serial.println("WROTE");
      } else {
        Serial.println("CUDNT WRITE");
      }

      delay(1000);

    } else {
      Serial.println("CUDNT AUTH");
    }
  } else {
    Serial.println("NO");
  }
  delay(500);
}