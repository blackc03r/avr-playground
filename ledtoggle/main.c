#include <avr/io.h>
#include <stdbool.h>
#include "../common/pin.h"

/* Toggle LED with a button.
 *
 * - LED powered by PB4
 * - Push button, powered, connected to PD4
 * - Pulldown resistor on PD4
 *
 * LED starts off and its state is toggled when we press the push button.
 */

#define PinLED PinB4
#define PinButton PinD4

int main()
{
    bool button_was_pressed = false;
    bool led_is_on = false;

    pinoutputmode(PinLED);

    while (1) {
        if (pinishigh(PinButton)) {
            if (!button_was_pressed) {
                button_was_pressed = true;
                led_is_on = !led_is_on;
            }
        } else {
            button_was_pressed = false;
        }
        if (led_is_on) {
            pinhigh(PinLED);
        } else {
            pinlow(PinLED);
        }
    }
}
