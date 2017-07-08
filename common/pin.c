#include "pin.h"
#include "util.h"

void pinhigh(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            sbi(PORTB, pin & 0b111);
            break;
        case 0b10:
            sbi(PORTC, pin & 0b111);
            break;
        case 0b11:
            sbi(PORTD, pin & 0b111);
            break;
    }
}

void pinlow(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            cbi(PORTB, pin & 0b111);
            break;
        case 0b10:
            cbi(PORTC, pin & 0b111);
            break;
        case 0b11:
            cbi(PORTD, pin & 0b111);
            break;
    }
}

void pinset(Pin pin, bool high)
{
    if (high) {
        pinhigh(pin);
    } else {
        pinlow(pin);
    }
}

bool pinishigh(Pin pin)
{
    unsigned char portval = 0;
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            portval = PINB;
            break;
        case 0b10:
            portval = PINC;
            break;
        case 0b11:
            portval = PIND;
            break;
    }
    return (portval & (1 << (pin & 0b111))) != 0;
}

void pininputmode(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            cbi(DDRB, pin & 0b111);
            break;
        case 0b10:
            cbi(DDRC, pin & 0b111);
            break;
        case 0b11:
            cbi(DDRD, pin & 0b111);
            break;
    }
}

void pinoutputmode(Pin pin)
{
    switch ((pin >> 3) & 0b11) {
        case 0b01:
            sbi(DDRB, pin & 0b111);
            break;
        case 0b10:
            sbi(DDRC, pin & 0b111);
            break;
        case 0b11:
            sbi(DDRD, pin & 0b111);
            break;
    }
}

