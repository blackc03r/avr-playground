#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../common/pin.h"
#include "../common/util.h"
#include "../common/tone.h"
#include "../common/tune.h"

#define PinPiezo PinB4
#define PinButton1 PinD2
#define PinButton2 PinD3

/* Welcome to funkytown! if you can guess the secret code...
 *
 * - Piezo connected to PinB4
 * - Push button connected to PinD2
 * - Push button connected to PinD3
 *
 * Pushing buttons will play a tone until the button is released. Pushing the buttons
 * in the proper order will play funkytown!
 */

static Tune funkytown_notes = {
    NOTE_A4,
    NOTE_A4,
    NOTE_G4,
    NOTE_A4,
    NOTE_PAUSE,
    NOTE_E4,
    NOTE_PAUSE,
    NOTE_E4,
    NOTE_A4,
    NOTE_D5,
    NOTE_C5S,
    NOTE_A4,
    0,
};

ISR(INT0_vect) {
    stop_tone();
}

ISR(INT1_vect) {
    stop_tone();
}

int main()
{
    unsigned char code_sequence = 0;

    pinoutputmode(PinPiezo);

    TCCR1B = 0;
    sbi(EICRA, ISC00);
    sbi(EICRA, ISC10);
    sbi(EIMSK, INT0);
    sbi(EIMSK, INT1);
    sei();

    while (1) {
        if (pinishigh(PinButton2)) {
            tone(PinPiezo, NOTE_A4, 0);
            if (code_sequence != 2) {
                code_sequence++;
            } else {
                // Don't start back at 0: we've just played the first note!
                code_sequence = 1;
            }
        }
        if (pinishigh(PinButton1)) {
            tone(PinPiezo, NOTE_G4, 0);
            if (code_sequence == 2) {
                code_sequence++;
            } else {
                code_sequence = 0;
            }
        }
        if (code_sequence == 4) {
            code_sequence = 0;
            _delay_ms(300);
            play_tune(PinPiezo, funkytown_notes);
        }
    }
}
