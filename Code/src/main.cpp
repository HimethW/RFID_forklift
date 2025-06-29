#include <Arduino.h>

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define MISO PB3
#define MOSI PB2
#define SCK  PB1

// reset is digital 8, PB4
// busy is digital 9, PB5
// nss is digital 10, PB6

void send_bytes(char* bytes, int length) {
  for (int i = 0; i < length; i++) {
    SPDR = bytes[i];

    while (!(SPSR & (1 << SPIF))) {
      ;
    }
  }
}

void receive_bytes(char* buffer, int length) {
  for (int i = 0; i < length; i++) {
    SPDR = 0x00; // Send dummy byte to receive data

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

  DDRB |= (1 << DDB2) | (1 << DDB1); // Set MOSI, SCK as output
  DDRB |= (1 << DDB4) || (1 << DDB6); // Set SS, RST as output
  DDRB &= ~(1 << DDB5); // Set BUSY as input
  DDRC |= (1 << DDC7); // Set LED as output
  DDRB &= ~(1 << DDB3); // Set MISO as input

  PORTB |= (1 << PB6); // Set NSS high (inactive)
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); // Enable SPI, Set as Master, Prescaler: Fosc/16

  _delay_ms(50);

  Serial.println("RESETTING");

  PORTB &= ~(1 << PB4); // Set RST low to reset the device
  _delay_ms(100);
  PORTB |= (1 << PB4); // Set RST high to release the device from reset
  _delay_ms(100);
  
  Serial.println("RESET DONE");
}

void loop() {
  while (PINB & (1 << PB5)) {
    // Wait for BUSY to go low
  }

  PORTB &= ~(1 << PB6); // Set NSS low to select the device

  char send_data[3] = {0x07, 0x10, 0x02};
  send_bytes(send_data, 3);

  _delay_ms(50);

  while (!(PINB & (1 << PB5))) {
    // Wait for BUSY to go high
  }

  PORTB |= (1 << PB6); // Set NSS high to deselect the device

  _delay_ms(50);

  while (PINB & (1 << PB5)) {
    // Wait for BUSY to go low again
  }

  _delay_ms(500);
  // SEND COMPLETE

  while (PINB & (1 << PB5)) {
    // Wait for BUSY to go low
  }

  PORTB &= ~(1 << PB6); // Set NSS low to select the device

  char receive_buffer[2];
  receive_bytes(receive_buffer, 2);

  _delay_ms(50);

  while (!(PINB & (1 << PB5))) {
    // Wait for BUSY to go high
  }

  PORTB |= (1 << PB6); // Set NSS high to deselect the device

  while (PINB & (1 << PB5)) {
    // Wait for BUSY to go low again
  }

  if (receive_buffer[1] == 4) {
    PORTC |= (1 << PC7); // Turn on LED
    _delay_ms(1000);
    PORTC &= ~(1 << PC7); // Turn off LED
    _delay_ms(1000);
  }

  _delay_ms(1000);
}