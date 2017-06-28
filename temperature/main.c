#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../common/uart.h"

/* Outputs temperature to serial console.
 *
 * - TMP36, powered by 5V, grounded, connected to A0.
 *
 * Every second, prints a number to the console. That number is related to the temperature of the
 * termometer. The TMP36 outputs 750mA at 25 degrees Celsius. To make our measurement more accurate,
 * we use the 1.1V internal AREF rather than a 5V one. Considering that our ADC range is
 * 0-1024, that gives us 1.07 mV per step. This also means that we don't support temperature over
 * 60C.
 *
 * 5V or 1.1V are our only "easy" choices. For other references, we would have to wire some voltage
 * to the AREF pin, and from what I've read, it also requires a capacitor to ensure regularity. I
 * wouldn't know yet how to set this up...
 *
 * This code is the result of a *lot* of messing around, reading datasheets and tutorials. But
 * mostly messing around... But now it seems to work!
 *
 */

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/* MV == millivolt */
/* Input voltage being fed to the TMP36 chip */
#define TMP36_INPUT_VOLTAGE_MV 1100
/* Voltage that is supposed to be outputted by TMP36 at 25C */
#define TMP36_VOLTAGE_AT_25D_MV 750
/* TMP36's output is linearly related to temperature. Number of MV per degree. */
#define TMP36_VOLTAGE_PER_DEGREE_MV 10

/* Returns the degrees represented by a ADC reading.
 *
 * Returns the temperature in Celsius x10 (234 = 23.4C).
 *
 * adcval is a value in the 0-1023 range.
 */
int adcval2decidegrees(int adcval)
{
    int mv_read = (long)TMP36_INPUT_VOLTAGE_MV * (long)adcval / (long)1024;
    return 250 + (mv_read - TMP36_VOLTAGE_AT_25D_MV);
}

int main()
{
    uint8_t high, low;
    int adcval;
    int decidegrees;

    uart_init(9600);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    /* REFS0 == 5V AREF, REFS1 | REFS0 == 1.1V. Both flags unset == use voltage on the AREF pin. */
    /* We choose 1.1V. */
    ADMUX |= ((1 << REFS0) | (1 << REFS1));
    ADCSRA |= (1 << ADEN);

    while (1) {
        cbi(PRR, PRADC);
        sbi(ADCSRA, ADSC);
        while (bit_is_set(ADCSRA, ADSC));
        low = ADCL;
        high = ADCH;
        adcval = (high << 8) | low;
        decidegrees = adcval2decidegrees(adcval);
        printf("%d.%dC (%d)\n", decidegrees/10, decidegrees%10, adcval);
        _delay_ms(1000);
    }
}
