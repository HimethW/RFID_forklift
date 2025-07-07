#include "avr/io.h"
#include "SerialInterface.h"

SerialInterface::SerialInterface(unsigned long system_oscillator_frequency, unsigned long baud_rate) {
    // Configure registers to set the baud rate
    unsigned long UBRR = (system_oscillator_frequency / (16UL * baud_rate)) - 1;
    
    UBRR1L = (uint8_t)UBRR;
    UBRR1H = (uint8_t)(UBRR >> 8);

    // Enable receiver and transmitter
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);

    // Configure frame format as 8-N-1
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); // No parity and 1 stop bit are set by default
};

bool SerialInterface::send(uint8_t* bytes, int length) {
    for (int i = 0; i < length; i++) {
        while (!(UCSR1A & (1 << UDRE1))) {
            ; // Wait till USART Data Register is empty, to write new data to the transmit buffer
        }

        UDR1 = (uint8_t)bytes[i];
    }
    
    return true;
};

bool SerialInterface::receive(uint8_t* bytes, int length) {
    for (int i = 0; i < length; i++) {
        while (!(UCSR1A & (1 << RXC1))) {
            ; // Wait until the USART Receive Complete bit is set indicating there is unread data in UDR
        }

        bytes[i] = UDR1;
    }

    return true;
};