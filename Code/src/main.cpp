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
}

void loop() {

  uint8_t send_data[3] = { 0x07, 0x10, 0x02 };
  nfc.transceive(SEND, send_data, 3);  // Send 3 bytes
  Serial.println("send complete");

  uint8_t receive_buffer[2];
  nfc.transceive(RECEIVE, receive_buffer, 2);  // Receive 2 bytes
  Serial.println("receive complete");
  
  Serial.print("I RECEIVED ");
  Serial.println(receive_buffer[1]);
  Serial.println("!!!!!!!!!!!!!!!!");
  delay(2000);

  if (receive_buffer[1] == 4) {
    LED.assert();  // Turn on LED
    _delay_ms(1000);
    LED.deassert();  // Turn off LED
    _delay_ms(1000);
  }

  _delay_ms(1000);
}