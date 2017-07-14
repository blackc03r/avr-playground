#include "tune.h"
#include <util/delay.h>
#include "tone.h"

static void beep(Pin pin, unsigned int frequency)
{
    tone(pin, frequency, 200);
    _delay_ms(100);
}

static void pause()
{
    _delay_ms(300);
}

void play_tune(Pin pin, Tune tune)
{
    unsigned char i;

    for (i=0; ; i++) {
        if (tune[i] == 0) {
            break;
        } else if (tune[i] == NOTE_PAUSE) {
            pause();
        } else {
            beep(pin, tune[i]);
        }
    }
}


