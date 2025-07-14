/*
    SerialInterface.h

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Simplfies the usage of the ATMega328P's hardware USART.

    The following sources were referenced.

    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
*/

#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#ifndef CPU_FREQ
#define CPU_FREQ    16000000 // 16 MHz
#endif

// Relevant registers and bit positions [Section 31]
#define UCSR0A      *((volatile unsigned char*)(0xC0))
#define UCSR0B      *((volatile unsigned char*)(0xC1))
#define UCSR0C      *((volatile unsigned char*)(0xC2))

#define UBRR0L      *((volatile unsigned char*)(0xC4))
#define UBRR0H      *((volatile unsigned char*)(0xC5))
#define UDR0        *((volatile unsigned char*)(0xC6))

#define UCSZ00      1
#define UCSZ01      2
#define TXEN0       3
#define RXEN0       4
#define UDRE0       5
#define RXC0        7

class SerialInterface {
    public:
        SerialInterface(unsigned long baud_rate);
        bool send(unsigned char* send_bytes, int num_bytes);
        bool receive(unsigned char* receive_buffer, int num_bytes);
};

#endif