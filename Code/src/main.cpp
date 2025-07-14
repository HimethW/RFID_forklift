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
  delay_ms(1000);
  Serial.println("HI");

  pn532.initialize();
  delay_ms(100);

  pn532.SAMConfig();

  initialize_timer_0(PRESCALER_64);
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

      delay_ms(2000);

      Serial.println("GONNA WRITE");
      
      unsigned char newcont[16] = {5, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
      
      if (card->write_block(0x02, newcont)) {
        Serial.println("WROTE");
      } else {
        Serial.println("CUDNT WRITE");
      }

      delay_ms(1000);

    } else {
      Serial.println("CUDNT AUTH");
    }
  } else {
    Serial.println("NO");
  }
  delay_ms(500);
}