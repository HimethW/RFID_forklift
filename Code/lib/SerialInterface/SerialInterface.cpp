/*
    SerialInterface.cpp

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Simplfies the usage of the ATMega328P's hardware USART.

    The following sources were referenced.

    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*/

#include "SerialInterface.h"

SerialInterface::SerialInterface(unsigned long system_oscillator_frequency, unsigned long baud_rate) {
    // Following the example in Section 19.5

    // Calculate UBRR and configure the registers to set the baud rate [Table 19-1, Section 19.3.1], [Section 19.10.5]
    unsigned long UBRR = (system_oscillator_frequency / (16UL * baud_rate)) - 1;
    
    UBRR0L = (unsigned char)UBRR;
    UBRR0H = (unsigned char)(UBRR >> 8);

    // Enable receiver and transmitter [Section 19.10.3]
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Configure frame format as 8-N-1 [Section 19.10.4]
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // No parity and 1 stop bit are set by default
};

bool SerialInterface::send(unsigned char* bytes, int length) {
    // With reference to the example in Section 19.6.1 and Section 19.10.2
    for (int i = 0; i < length; i++) {
        while (!(UCSR0A & (1 << UDRE0))) {
            ; // Wait till USART Data Register is empty, to write new data to the transmit buffer
        }

        UDR0 = (unsigned char)bytes[i]; // Send a byte
    }
    
    return true;
};

bool SerialInterface::receive(unsigned char* bytes, int length) {
    // With reference to the example in Section 19.7.1 and Section 19.10.2
    for (int i = 0; i < length; i++) {
        while (!(UCSR0A & (1 << RXC0))) {
            ; // Wait until the USART Receive Complete bit is set indicating there is unread data in UDR
        }

        bytes[i] = UDR0; // Read a byte
    }

    return true;
};