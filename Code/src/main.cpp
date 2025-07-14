#include <Arduino.h>
#include <Pins.h>
#include <Timer.h>
#include <SerialInterface.h>
#include <SPI.h>
#include <PN532.h>

Pin HW_MOSI(B, 2);
Pin HW_MISO(B, 3);
Pin HW_SCK(B, 1);
Pin NSS(B, 0);

PN532 pn532(NSS, HW_MOSI, HW_MISO, HW_SCK);

void setup() {
  Serial.begin(9600);
  blocking_delay(1000, MILLISECONDS);
  Serial.println("HI");

  pn532.initialize();
  blocking_delay(100, MILLISECONDS);

  pn532.SAMConfig();

  initialize_timer();
}

void loop() {
  MIFARE_Classic_PN532* card;
  card = pn532.get_mifare_classic_card();
  if (card) {
    Serial.println("FOUND");
    unsigned char key[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    bool ok = card->authenticate_block(AUTHENTICATE_KEY_A, 0x02, key);
    if (ok) {
      Serial.println("AUTHED");
      
      unsigned char contents[16];
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

      blocking_delay(2000, MILLISECONDS);

      Serial.println("GONNA WRITE");
      
      unsigned char newcont[16] = {5, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
      
      if (card->write_block(0x02, newcont)) {
        Serial.println("WROTE");
      } else {
        Serial.println("CUDNT WRITE");
      }

      blocking_delay(1000, MILLISECONDS);

    } else {
      Serial.println("CUDNT AUTH");
    }
  } else {
    Serial.println("NO");
  }
  blocking_delay(500, MILLISECONDS);
}