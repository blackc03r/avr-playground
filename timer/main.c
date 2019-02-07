#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../common/pin.h"
#include "../common/util.h"

/* Blink using timer0 instead of _delay_ms() */

#define USECS_PER_SECOND 1000000UL
#define BLINK_DELAY_MS 200
#define PIN PinB5

static bool ishigh;
static int counter;

static unsigned long usecs_to_ticks(unsigned long usecs)
{
    unsigned long r;
    if (F_CPU >= USECS_PER_SECOND) {
        r = (F_CPU / USECS_PER_SECOND) * usecs;
    } else {
        r = (F_CPU * usecs) / USECS_PER_SECOND;
        if (r == 0) {
            r = 1;
        }
    }
    return r;
}

bool set_timer0_target(unsigned long usecs)
{
    unsigned char prescaler_shifts[5] = {0, 3, 6, 8, 10};
    unsigned char prescaler_bits[5] = {0b001, 0b010, 0b011, 0b100, 0b101};
    unsigned char prescaler_index;
    unsigned long ticks;

    if (usecs == 0) {
        // Stop the timer.
        TCCR0B &= 0b11111000;
        OCR0A = 0;
        return true;
    }

    ticks = usecs_to_ticks(usecs);

    for (prescaler_index=0; prescaler_index<=5; prescaler_index++) {
        if (ticks >> prescaler_shifts[prescaler_index] <= 0xff) {
            break;
        }
    }
    if (prescaler_index > 5) {
        return false;
    }
    // Set CS10, CS11 and CS12 according to our selected prescaler bits
    TCCR0B &= 0b11111000;
    TCCR0B |= prescaler_bits[prescaler_index];
    OCR0A = ticks >> prescaler_shifts[prescaler_index];
    return true;
}

ISR(TIMER0_COMPA_vect)
{
    counter++;
    if (counter == BLINK_DELAY_MS) {
        counter = 0;
        ishigh = !ishigh;
    }
    pinset(PIN, ishigh);
}

int main(void)
{
    sei();
    // Enable CTC mode
    sbi(TCCR0A, WGM01);
    // Enable interrupt on compare match on OCR0A.
    sbi(TIMSK0, OCIE0A);
    // 1ms. at 16mhz, our time overflows very fast, so we can't wait for
    // BLINK_DELAY_MS directly;
    set_timer0_target(1000UL);
    pinoutputmode(PIN);
    ishigh = false;
    counter = 0;

    while(1);
}
