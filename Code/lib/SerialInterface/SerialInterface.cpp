#include "avr/io.h"
#include "SerialInterface.h"

SerialInterface::SerialInterface(unsigned long system_oscillator_frequency, unsigned long baud_rate) {
    // Configure registers to set the baud rate
    unsigned long UBRR = (system_oscillator_frequency / (16UL * baud_rate)) - 1;
    
    UBRR0L = (uint8_t)UBRR;
    UBRR0H = (uint8_t)(UBRR >> 8);

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Configure frame format as 8-N-1
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // No parity and 1 stop bit are set by default
};

bool SerialInterface::send(uint8_t* bytes, int length) {
    for (int i = 0; i < length; i++) {
        while (!(UCSR0A & (1 << UDRE0))) {
            ; // Wait till USART Data Register is empty, to write new data to the transmit buffer
        }

        UDR0 = (uint8_t)bytes[i];
    }
    
    return true;
};

bool SerialInterface::receive(uint8_t* bytes, int length) {
    for (int i = 0; i < length; i++) {
        while (!(UCSR0A & (1 << RXC0))) {
            ; // Wait until the USART Receive Complete bit is set indicating there is unread data in UDR
        }

        bytes[i] = UDR0;
    }

    return true;
};