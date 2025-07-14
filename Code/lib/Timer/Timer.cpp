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

#include "Timer.h"

void initialize_timer_0(unsigned char prescaler) {
    /*
        Initialize Timer 0 with the specified prescaler
    */

    // Keep OC0A and OC0B disconnected
    TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0));
    TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0));

    // Set WGM bits to configure Timer 0 in the CTC mode
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    TCCR0B &= 0xF8; // Clear the CS bits in TCCR0B
    TCCR0B |= prescaler; // Set the prescaler
};

void delay_ms(unsigned long milliseconds) {
    /*
        Delay for the specified number of milliseconds using Timer 0
    */

    // Set TCNT = 0 and initialize OCR0A
    TCNT0 = 0;
    OCR0A = 250;

    float tick_period; // In milliseconds
    switch (TCCR0B & 0x07) { // Extract CS bits from TCCR0B
        case NO_PRESCALER:
            tick_period = 1000.0 / CPU_FREQ;
            break;
        case PRESCALER_8:
            tick_period = 8000.0 / CPU_FREQ;
            break;
        case PRESCALER_64:
            tick_period = 64000.0 / CPU_FREQ;
            break;
        case PRESCALER_256:
            tick_period = 256000.0 / CPU_FREQ;
            break;
        case PRESCALER_1024:
            tick_period = 1024000.0 / CPU_FREQ;
            break;
        default:
            return; // Invalid prescaler
    }

    unsigned long ticks_to_count = milliseconds / (tick_period * 250);
    unsigned long ticks_so_far = 0;

    while (ticks_so_far < ticks_to_count) {
        // Wait for the Output Compare Match A Interrupt to be triggered
        while (!(TIFR0 & (1 << OCF0A)));

        // Clear the interrupt flag
        TIFR0 |= (1 << OCF0A);

        ticks_so_far++;
    }

    return;
};