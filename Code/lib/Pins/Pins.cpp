#include "avr/io.h"
#include "Pins.h"

Pin::Pin(uint8_t port, uint8_t pin) {
    _port = port;
    _pin = pin;
};

void Pin::set_output() {
    switch (_port) {
        case B:
            DDRB |= (1 << _pin);
            break;
        case C:
            DDRC |= (1 << _pin);
            break;
        case D:
            DDRD |= (1 << _pin);
            break;
    }
}

void Pin::set_input() {
    switch (_port) {
        case B:
            DDRB &= ~(1 << _pin);
            break;
        case C:
            DDRC &= ~(1 << _pin);
            break;
        case D:
            DDRD &= ~(1 << _pin);
            break;
    }
};

void Pin::assert() {
    switch (_port) {
        case B:
            PORTB |= (1 << _pin);
            break;
        case C:
            PORTC |= (1 << _pin);
            break;
        case D:
            PORTD |= (1 << _pin);
            break;
    }
};

void Pin::deassert() {
    switch (_port) {
        case B:
            PORTB &= ~(1 << _pin);
            break;
        case C:
            PORTC &= ~(1 << _pin);
            break;
        case D:
            PORTD &= ~(1 << _pin);
            break;
    }
};

void Pin::toggle() {
    switch (_port) {
        case B:
            PORTB ^= (1 << _pin);
            break;
        case C:
            PORTC ^= (1 << _pin);
            break;
        case D:
            PORTD ^= (1 << _pin);
            break;
    }
};

bool Pin::state() {
    switch (_port) {
        case B:
            return (PINB & (1 << _pin));
        case C:
            return (PINC & (1 << _pin));
        case D:
            return (PIND & (1 << _pin));
        default:
            return false; // Invalid port
    }
};

bool Pin::is_low() {
    return !state();
};

bool Pin::is_high() {
    return state();
};