#include <avr/io.h>

/* A LED with its intensity being controlled by a knob.
 *
 * - LED connected to PB1 (PWM)
 * - Potentiometer (knob) connected to ADC0
 *
 * This program continuously reads samples from the potentiometer and adjusts the duty cycle of
 * PB1 accordingly, thus making the light brighter or dimmer depending on the knob's position.
 *
 */

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int sample_knob()
{
    uint8_t high, low;
    cbi(PRR, PRADC);
    sbi(ADCSRA, ADSC);
    while (bit_is_set(ADCSRA, ADSC));
    low = ADCL;
    high = ADCH;
    return (high << 8) | low;
}

int main()
{
    int adcval;

    /* Set PB1 for output */
    sbi(DDRB, DDB1);

    /* Setup PB1 for PWM */
    /* Ref: http://www.electroschematics.com/9941/avr-pwm/ */
    OCR1A = 0;
    sbi(TCCR1A, COM1A1);
    sbi(TCCR1A, WGM10);
    sbi(TCCR1A, WGM11);
    sbi(TCCR1B, CS11);

    /* Setup A0 for analog reading */
    sbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS2);

    /* 5V AREF */
    sbi(ADMUX, REFS0);

    sbi(ADCSRA, ADEN);

    while (1) {
        adcval = sample_knob();
        OCR1A = adcval;
    }
}
