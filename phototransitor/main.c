#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../common/util.h"
#include "../common/uart.h"

/* Outputs phototransistor output to serial console.
 *
 * - A phototransistor, powered by 5V, grounded, connected to A0.
 *
 * Every second, prints a number to the console. That number is an indicator of ambient light
 * level and has a 0-1023 range.
 *
 */

int main()
{
    uint8_t high, low;
    int adcval;

    uart_init(9600);
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    /* REFS0 == 5V AREF, REFS1 | REFS0 == 1.1V. Both flags unset == use voltage on the AREF pin. */
    /* We choose 5V. */
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADEN);

    while (1) {
        cbi(PRR, PRADC);
        sbi(ADCSRA, ADSC);
        while (bit_is_set(ADCSRA, ADSC));
        low = ADCL;
        high = ADCH;
        adcval = (high << 8) | low;
        printf("%d\n", adcval);
        _delay_ms(1000);
    }
}
