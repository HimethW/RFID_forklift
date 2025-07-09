#include <Pins.h>
#include <SerialInterface.h>
#include <SPI.h>
#include <PN532.h>

#include <avr/io.h>
#include <avr/delay.h>

Pin buzzer(D, 7);

Pin HW_MOSI(B, 3);
Pin HW_MISO(B, 4);
Pin HW_SCK(B, 5);
Pin NSS(B, 2);

PN532 pn532(NSS, HW_MOSI, HW_MISO, HW_SCK);

SerialInterface mySerial(16000000UL, 9600);

void ready_tone() {
  buzzer.assert();
  _delay_ms(100);
  buzzer.deassert();
  _delay_ms(100);
  buzzer.assert();
  _delay_ms(100);
  buzzer.deassert();
  _delay_ms(100);
}

int main() {
  buzzer.set_output();

  pn532.initialize();
  _delay_ms(100);

  pn532.SAMConfig();

  ready_tone();

  while (true) {
    uint8_t tag_num;
    uint8_t tag_data[8];
    
    if (pn532.detect_tag(&tag_num, tag_data)) {
      buzzer.assert();
      _delay_ms(300);
    }

    buzzer.deassert();
  }

  return 0;
}