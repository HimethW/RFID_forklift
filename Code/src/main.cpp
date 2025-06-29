#include <Arduino.h>
#include <Pins.h>
#include <SPI.h>

#include <avr/io.h>
#include <avr/delay.h>

Pin HW_MOSI(B, 2);
Pin HW_MISO(B, 3);
Pin HW_SCK(B, 1);

SPI_Master myspi(HW_MOSI, HW_MISO, HW_SCK);

Pin RST(B, 4);  // Digital Pin 8
Pin BUSY(B, 5); // Digital Pin 9
Pin NSS(B, 6);  // Digital Pin 10

Pin LED(C, 7);  // L LED
/*
void send_bytes(int* bytes, int length) {
  for (int i = 0; i < length; i++) {
    SPDR = bytes[i];
    Serial.print(bytes[i]);
    Serial.print(" (");

    while (!(SPSR & (1 << SPIF))) {
      ;
    }

    int rec_byte = SPDR;
    Serial.print(rec_byte);
    Serial.print(") ");
  }
  Serial.println("");
}
*/
void receive_bytes(int* buffer, int length) {
  for (int i = 0; i < length; i++) {
    SPDR = 0x00;  // Send dummy byte to receive data

    while (!(SPSR & (1 << SPIF))) {
      ;
    }

    buffer[i] = SPDR;
  }
}

void setup() {
  Serial.begin(115200);

  delay(2000);
  Serial.println("HELLO");
  delay(1000);

  PRR0 &= ~(1 << PRSPI);

  RST.set_output();
  NSS.set_output();
  BUSY.set_input();
  
  LED.set_output();

  NSS.assert();  // Set NSS high (inactive)
  
  myspi.initialize();  // Initialize SPI

  _delay_ms(50);

  Serial.println("RESETTING");

  RST.deassert();  // Set RST low to reset the device
  _delay_ms(100);
  RST.assert();  // Set RST high to release the device from reset
  _delay_ms(100);

  Serial.println("RESET DONE");
}

void loop() {
  Serial.println("waiting for busy to go low");
  while (BUSY.state()) {
    // Wait for BUSY to go low
  }
  Serial.println("busy is low");

  NSS.deassert();  // Set NSS low to select the device
  Serial.println("nss set to low");

  uint8_t send_data[3] = { 0x07, 0x10, 0x02 };
  myspi.send(send_data, 3);

  Serial.println("data sent");

  Serial.println("waiting for busy to go high");
  while (!BUSY.state()) {
    // Wait for BUSY to go high
  }
  Serial.println("busy is high");

  NSS.assert();  // Set NSS high to deselect the device
  Serial.println("nss set to high, waiting for busy to go low");
  while (BUSY.state()) {
    // Wait for BUSY to go low again
  }
  Serial.println("busy is low");

  _delay_ms(500);
  // SEND COMPLETE
  Serial.println("send complete");

  while (BUSY.state()) {
    // Wait for BUSY to go low
  }

  NSS.deassert();  // Set NSS low to select the device again

  uint8_t receive_buffer[2];
  // receive_bytes(receive_buffer, 2);
  myspi.receive(receive_buffer, 2);  // Receive 2 bytes 

  _delay_ms(50);

  while (!BUSY.state()) {
    // Wait for BUSY to go high
  }

  NSS.assert();  // Set NSS high to deselect the device

  while (BUSY.state()) {
    // Wait for BUSY to go low again
  }

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