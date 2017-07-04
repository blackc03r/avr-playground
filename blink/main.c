#include <avr/io.h>
#include <util/delay.h>
#include "../common/pin.h"

/* Make the builtin LED blink.
 */

#define BLINK_DELAY_MS 1000

int main (void)
{
    /* PinB5 is the builtin LED */
    pinoutputmode(PinB5);

    while(1) {
        pinhigh(PinB5); // turn on
        _delay_ms(BLINK_DELAY_MS);
        pinlow(PinB5); // turn off
        _delay_ms(BLINK_DELAY_MS);
    }
}
