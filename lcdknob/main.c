#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "../common/util.h"
#include "../common/pin.h"

/* A LCD screen with its content being controlled by buttons and a knob.
 *
 * - A LCD screen with 11 pins connected to our board, sequentially.
 *   We start with RS connected at D2 (pin 2) and we end with DB7
 *   connected to B5 (pin 12).
 *
 * - A 10k potentiometer, on the 5V, connected to ADC0
 *
 * On startup, our LCD will display "Hello!" with its cursor next to the "!". Then comes the
 * "knob-controlled" character. Its value is determined by the position of the potentiometer
 * and will be changed in real time. Is that not super cool?
 */

#define PinRS PinD2
#define PinRW PinD3
#define PinE PinD4
#define PinDB0 PinD5
#define PinDB1 PinD6
#define PinDB2 PinD7
#define PinDB3 PinB0
#define PinDB4 PinB1
#define PinDB5 PinB2
#define PinDB6 PinB3
#define PinDB7 PinB4

/* The number of "same value" we need to have on ADC0 to consider it stable.
 * This is to prevent flickering when the knob is exactly at the edge of a value.
 */
#define ANTI_FLICKER_THRESHOLD 10

static unsigned char data_pins[8] = {
    PinDB0,
    PinDB1,
    PinDB2,
    PinDB3,
    PinDB4,
    PinDB5,
    PinDB6,
    PinDB7,
};

static void write8bit(unsigned char val)
{
    unsigned char i;
    for (i=0; i<8; i++) {
        if (val & (1 << i)) {
            pinhigh(data_pins[i]);
        } else {
            pinlow(data_pins[i]);
        }
    }
}

static void clearbytes()
{
    write8bit(0);
}

static void pulseEnable()
{
    pinlow(PinE);
    _delay_us(1);
    pinhigh(PinE);
    _delay_us(1);
    pinlow(PinE);
}

// Returns whether the BF flag was ON at least for one cycle
static void wait_for_bf()
{
    // We don't use pulseEnable() because PinE has to be HIGH when we read the BF flag.
    pinlow(PinRS);
    pinhigh(PinRW);
    pininputmode(PinDB7);
    pinhigh(PinE);
    _delay_us(1);
    while (pinishigh(PinDB7)) {
        pinlow(PinE);
        _delay_us(1);
        pinhigh(PinE);
        _delay_us(1);
    }
    pinlow(PinE);
    pinoutputmode(PinDB7);
    pinlow(PinRW);
}

static void pulseEnableAndWaitBF()
{
    pulseEnable();
    wait_for_bf();
}

static void sendcmd(unsigned char cmd)
{
    pinlow(PinRS);
    write8bit(cmd);
    pulseEnableAndWaitBF();
}

static void sendchar(unsigned char c)
{
    pinhigh(PinRS);
    write8bit(c);
    pulseEnableAndWaitBF();
}

static unsigned char sample_knob()
{
    cbi(PRR, PRADC);
    sbi(ADCSRA, ADSC);
    while (bit_is_set(ADCSRA, ADSC));
    return ADCH;
}

void set_rolling_char(unsigned char val) {
    sendchar(val);
    // make cursor go back
    sendcmd(0b00010000);
}

/* Read knob on ADC0 and changes the last letter on our LCD accordingly.
 */
static void knobroll_forever()
{
    unsigned char currentval = 0;
    unsigned char candidateval = 0;
    unsigned char sameval_counter = 0;
    unsigned char sample;

    sample = sample_knob();
    set_rolling_char(sample);
    currentval = sample;

    while (1) {
        sample = sample_knob();
        if (sample == currentval) {
            sameval_counter = 0;
        } else if (sample == candidateval) {
            sameval_counter++;
            if (sameval_counter == ANTI_FLICKER_THRESHOLD) {
                currentval = sample;
                sameval_counter = 0;
                set_rolling_char(sample);
            }
        } else {
            candidateval = sample;
            sameval_counter = 0;
        }
    }
}

int main()
{
    pinoutputmode(PinRS);
    pinoutputmode(PinE);
    pinoutputmode(PinRW);
    pinoutputmode(PinDB0);
    pinoutputmode(PinDB1);
    pinoutputmode(PinDB2);
    pinoutputmode(PinDB3);
    pinoutputmode(PinDB4);
    pinoutputmode(PinDB5);
    pinoutputmode(PinDB6);
    pinoutputmode(PinDB7);
    clearbytes();
    pinlow(PinRS);
    pinlow(PinE);

    // Initialization
    _delay_ms(40);
    write8bit(0b00110000);
    pulseEnable();
    _delay_ms(5);
    pulseEnable();
    _delay_us(100);

    // Display mode
    sendcmd(0b00001110);

    // display clear
    sendcmd(0b00000001);

    // entry mode
    sendcmd(0b00000110);

    // Display mode
    sendcmd(0b00001110);

    // write text
    sendchar('H');
    sendchar('e');
    sendchar('l');
    sendchar('l');
    sendchar('o');
    sendchar('!');

    // Setup ADC0 (knob)
    sbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS2);
    sbi(ADMUX, REFS0); // 5V AREF
    sbi(ADMUX, ADLAR); // 8-bit mode
    sbi(ADCSRA, ADEN); // Go!

    knobroll_forever();
}
