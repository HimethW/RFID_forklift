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

void initialize_timer() {
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

    // Clear the CS bits (3 least significant bits) in TCCR0B to disable the timer
    TCCR0B &= 0b11111000;
};

bool blocking_delay(unsigned long duration, unsigned char unit) {
    /*
        Delay for the specified number of milliseconds using Timer 0
    */

    TCNT0 = 0;

    /*
        The timer period is (Prescaling Factor / CPU_FREQ), i.e., TCNT0 is incremented by 1, every (Prescaling Factor / CPU_FREQ)
        seconds. The OCF0A flag is set whenever TCNT0 reaches the value in OCR0A, so if TCNT0 starts at 0, the time taken for it to
        reach the value in OCR0A is (Prescaling Factor / CPU_FREQ) * OCR0A seconds.

        If we reset TCNT0 every time it reaches OCR0A (which we can detect when the OCF0A flag is set), we can count in increments
        of (Prescaling Factor / CPU_FREQ) * OCR0A seconds.

        The smallest time increment we can count is when the prescaling factor and OCR0A are both 1 (their smallest meaningful values),
        and is (1 / CPU_FREQ) seconds, or 62.5 nanoseconds.
        The largest time increment we can count is when the prescaling factor is 1024 and OCR0A is 255, and is (1024 / CPU_FREQ) * 255
        = 16.384 milliseconds.

        Hence, we define two levels of precision; millisecond and microsecond precision.
        
        For millisecond precision, we need (Prescaling Factor / CPU_FREQ) * OCR0A = 0.001 seconds, so Prescaling Factor * OCR0A =
        16000000 * 0.001 = 16000. But 16000 = 64 * 250, so we can use a prescaling factor of 64 and OCR0A = 250.

        For microsecond precision, we need (Prescaling Factor / CPU_FREQ) * OCR0A = 0.000001 seconds, so Prescaling Factor * OCR0A =
        16000000 * 0.000001 = 16, so we can use a prescaling factor of 1 and OCR0A = 16.
    */
    if (unit == MILLISECONDS) {
        TCCR0B |= PRESCALER_64;
        OCR0A = 250;
    } else if (unit == MICROSECONDS) {
        TCCR0B |= NO_PRESCALER;
        OCR0A = 16;
    } else {
        return false;
    }

    unsigned long ticks_counted = 0;

    while (ticks_counted < duration) {
        // Blocking wait until TCNT0 reaches OCR0A
        while (!(TIFR0 & (1 << OCF0A)));

        // Clear the OCF0A flag
        TIFR0 |= (1 << OCF0A);

        ticks_counted++;
    }

    return true;
};