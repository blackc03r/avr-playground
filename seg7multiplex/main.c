#include <avr/io.h>
#include <util/delay.h>
#include "../common/pin.h"

/* A quick multiplex experiment I've hacked together with 7 segments
 *
 * I want to see what kind of delay is visually acceptable
 */

// When going over 10ms, a flicker begins to be visible (at least for me).
// that tells me that, with a limit like this, it's very comfortable to use
// something like a shift register to feed 7 segments displays in a multiplex
// scheme because filling 8 bits of a SR takes less than 1ms.
#define DELAY_US 10000

int main (void)
{
    pinoutputmode(PinB3); // anode 1
    pinoutputmode(PinB4); // anode 2
    pinoutputmode(PinD2); // common seg 1
    pinoutputmode(PinD3); // common seg 2

    // first 7seg has seg 1 low (on) and second 7seg has seg 2 low
    while(1) {
        pinhigh(PinD3);
        pinlow(PinB4);
        pinlow(PinD2);
        pinhigh(PinB3);
        _delay_us(DELAY_US);
        pinhigh(PinD2);
        pinlow(PinB3);
        pinlow(PinD3);
        pinhigh(PinB4);
        _delay_us(DELAY_US);
    }
}
