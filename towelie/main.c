#include <avr/io.h>
#include <util/delay.h>
#include "../common/pin.h"
#include "../common/util.h"

#define PinPiezo PinB4

/* frequency: hertz
 * duration: millis
 */
void tone(unsigned int frequency, unsigned int duration)
{
    unsigned long target_counter_reset;
    unsigned char prescaler_shifts[5] = {0, 3, 6, 8, 10};
    unsigned char prescaler_bits[5] = {0b001, 0b010, 0b011, 0b100, 0b101};
    unsigned char prescaler_index;
    unsigned int prescaled_counter_reset;
    unsigned long toggle_count;
    bool piezo_is_on = false;

    // We divide by 2 because our frequency represent a whole cycle (on, then off)
    target_counter_reset = F_CPU / frequency / 2;
    for (prescaler_index=0; prescaler_index<=5; prescaler_index++) {
        if (target_counter_reset >> prescaler_shifts[prescaler_index] <= 0xffff) {
            break;
        }
    }
    prescaled_counter_reset = target_counter_reset >> prescaler_shifts[prescaler_index];
    toggle_count = 2 * (long)frequency * (long)duration / 1000;

    // Set CS10, CS11 and CS12 according to our selected prescaler bits
    TCCR1B |= prescaler_bits[prescaler_index];

    while (1) {
        if (TCNT1 >= prescaled_counter_reset) {
            piezo_is_on = !piezo_is_on;
            pinset(PinPiezo, piezo_is_on);
            TCNT1 = 0;
            toggle_count--;
            if (toggle_count == 0) {
                break;
            }
        }
    }
    pinlow(PinPiezo);
}

void beep(unsigned int frequency)
{
    tone(frequency, 200);
    _delay_ms(100);
}

void pause()
{
    _delay_ms(300);
}

void funkytown()
{
    beep(440);
    beep(440);
    beep(392);
    beep(440);
    pause();
    beep(330);
    pause();
    beep(330);
    beep(440);
    beep(587);
    beep(554);
    beep(440);
}
int main (void)
{
    pinoutputmode(PinPiezo);

    TCCR1B = 0;

    funkytown();
}
