#include <avr/io.h>
#include <util/delay.h>
#include "../common/pin.h"

/* Make the builtin LED blink.
 */

#define BLINK_DELAY_MS 1000
#define PIN PinB5

int main (void)
{
    /* PinB5 is the builtin LED */
    pinoutputmode(PIN);

    while(1) {
        pinhigh(PIN); // turn on
        _delay_ms(BLINK_DELAY_MS);
        pinlow(PIN); // turn off
        _delay_ms(BLINK_DELAY_MS);
    }
}
