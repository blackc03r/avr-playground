#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../common/pin.h"
#include "../common/util.h"

/* Blink for every 5 oscillation (using a push button for example) on PD4.
 * This uses counter functionality (PD4 is T0, this is why we use this pin).
 * Don't forget the pulldown on PD4 */

#define BLINK PinB5
#define BLINK_EVERY 5

static bool ishigh;

ISR(TIMER0_COMPA_vect)
{
    ishigh = !ishigh;
    pinset(BLINK, ishigh);
}

int main(void)
{
    sei();
    // Enable CTC mode
    sbi(TCCR0A, WGM01);
    // Enable T0 listen mode
    sbi(TCCR0B, CS02);
    sbi(TCCR0B, CS01);
    sbi(TCCR0B, CS00);
    // Interrupt triggers when TCNT0 reaches BLINK_EVERY
    OCR0A = BLINK_EVERY - 1;
    // Enable interrupt on compare match on OCR0A.
    sbi(TIMSK0, OCIE0A);
    pinoutputmode(BLINK);
    ishigh = false;

    while(1);
}
