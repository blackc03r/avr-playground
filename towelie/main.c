#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../common/pin.h"
#include "../common/util.h"

#define PinPiezo PinB4
#define PinButton1 PinD2
#define PinButton2 PinD3

#define NOTE_E4 330
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_C5S 554
#define NOTE_D5 587

/* Welcome to funkytown! if you can guess the secret code...
 *
 * - Piezo connected to PinB4
 * - Push button connected to PinD2
 * - Push button connected to PinD3
 *
 * Pushing buttons will play a tone until the button is released. Pushing the buttons
 * in the proper order will play funkytown!
 */

static volatile bool tone_was_interrupted = false;

/* frequency: hertz
 * duration: millis, 0 == forever, until with stop_tone() through an interrupt
 */
static void tone(unsigned int frequency, unsigned int duration)
{
    unsigned long target_counter_reset;
    unsigned char prescaler_shifts[5] = {0, 3, 6, 8, 10};
    unsigned char prescaler_bits[5] = {0b001, 0b010, 0b011, 0b100, 0b101};
    unsigned char prescaler_index;
    unsigned int prescaled_counter_reset;
    unsigned long toggle_count;
    bool piezo_is_on = false;

    tone_was_interrupted = false;

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
            if (duration > 0) {
                toggle_count--;
                if (toggle_count == 0) {
                    break;
                }
            }
            else if (tone_was_interrupted) {
                tone_was_interrupted = false;
                break;
            }
        }
    }
    pinlow(PinPiezo);
}

static void stop_tone() {
    tone_was_interrupted = true;
}

static void beep(unsigned int frequency)
{
    tone(frequency, 200);
    _delay_ms(100);
}

static void pause()
{
    _delay_ms(300);
}

ISR(INT0_vect) {
    stop_tone();
}

ISR(INT1_vect) {
    stop_tone();
}

static void funkytown()
{
    beep(NOTE_A4);
    beep(NOTE_A4);
    beep(NOTE_G4);
    beep(NOTE_A4);
    pause();
    beep(NOTE_E4);
    pause();
    beep(NOTE_E4);
    beep(NOTE_A4);
    beep(NOTE_D5);
    beep(NOTE_C5S);
    beep(NOTE_A4);
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
            tone(NOTE_A4, 0);
            if (code_sequence != 2) {
                code_sequence++;
            } else {
                // Don't start back at 0: we've just played the first note!
                code_sequence = 1;
            }
        }
        if (pinishigh(PinButton1)) {
            tone(NOTE_G4, 0);
            if (code_sequence == 2) {
                code_sequence++;
            } else {
                code_sequence = 0;
            }
        }
        if (code_sequence == 4) {
            code_sequence = 0;
            pause();
            funkytown();
        }
    }
}
