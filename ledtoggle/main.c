#include <avr/io.h>
#include <stdbool.h>

/* Toggle LED with a button.
 *
 * - LED powered by PB4
 * - Push button, powered, connected to PD4
 * - Pulldown resistor on PD4
 *
 * LED starts off and its state is toggled when we press the push button.
 */
int main (void)
{
    bool button_was_pressed = false;
    bool led_is_on = false;

    // PB4 is for powering the LED
    DDRB |= _BV(DDB4);

    while (1) {
        if (PIND & _BV(PIND4)) {
            if (!button_was_pressed) {
                button_was_pressed = true;
                led_is_on = !led_is_on;
            }
        } else {
            button_was_pressed = false;
        }
        if (led_is_on) {
            PORTB |= _BV(PORTB4);
        } else {
            PORTB &= ~_BV(PORTB4);
        }
    }
}
