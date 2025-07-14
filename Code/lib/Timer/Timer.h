/*
    Timer.h

    compiled by Pulasthi Udugamasooriya, July 14, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Uses the timers on the ATMega328P for timing and delays.

    The following sources were referenced.

    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
    [Section 14]
*/

#ifndef TIMER_H
#define TIMER_H

// Relevant registers and bit positions [Section 31]
#define TIFR0       *((volatile unsigned char*)(0x35))

#define TCCR0A      *((volatile unsigned char*)(0x44))
#define TCCR0B      *((volatile unsigned char*)(0x45))

#define TCNT0       *((volatile unsigned char*)(0x46))
#define OCR0A       *((volatile unsigned char*)(0x47))

#define TIMSK0      *((volatile unsigned char*)(0x6E))

#define OCF0A           1

#define COM0B0          4
#define COM0B1          5
#define COM0A0          6
#define COM0A1          7

#define WGM00           0
#define WGM01           1
#define WGM02           3

#define CS00            0
#define CS01            1
#define CS02            2

#define OCIE0A          1

#define CPU_FREQ        16000000.0 // 16 MHz

#define NO_PRESCALER    0b001
#define PRESCALER_8     0b010
#define PRESCALER_64    0b011
#define PRESCALER_256   0b100
#define PRESCALER_1024  0b101

#define MILLISECONDS    0
#define MICROSECONDS    1

void initialize_timer();
bool blocking_delay(unsigned long milliseconds, unsigned char unit);

#endif