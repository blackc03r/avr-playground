#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "../common/util.h"
#include "../common/pin.h"

/* A LCD screen with its content being controlled by buttons and a knob.
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
#define PinLED PinB5

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

void write8bit(unsigned char val)
{
    char i;
    for (i=0; i<8; i++) {
        if (val & (1 << i)) {
            pinhigh(data_pins[i]);
        } else {
            pinlow(data_pins[i]);
        }
    }
}

void clearbytes()
{
    write8bit(0);
}

void debugLED(bool on)
{
    if (on) {
        pinhigh(PinLED);
    } else {
        pinlow(PinLED);
    }
}

void pulseEnable()
{
    pinlow(PinE);
    _delay_us(1);
    pinhigh(PinE);
    _delay_us(1);
    pinlow(PinE);
}

// Returns whether the BF flag was ON at least for one cycle
void wait_for_bf()
{
    // We don't use pulseEnable() because PinE has to be HIGH when we read the BF flag.
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

void pulseEnableAndWaitBF()
{
    pulseEnable();
    wait_for_bf(false);
}

void blink_forever(bool fast)
{
    while(1) {
        debugLED(true);
        if (fast) { _delay_ms(100); } else { _delay_ms(1000); }
        debugLED(false);
        if (fast) { _delay_ms(100); } else { _delay_ms(1000); }
    }
}

void sendcmd(unsigned char cmd)
{
    pinlow(PinRS);
    write8bit(cmd);
    pulseEnableAndWaitBF();
}

void sendchar(unsigned char c)
{
    pinhigh(PinRS);
    write8bit(c);
    pulseEnableAndWaitBF();
}

int main()
{
    pinoutputmode(PinLED);
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
    sendcmd(0b00001111);

    // write text
    sendchar('H');
    sendchar('e');
    sendchar('l');
    sendchar('l');
    sendchar('o');
    sendchar('!');

    // All pins low
    clearbytes();
    pinlow(PinRS);

    blink_forever(true);

    // Return home
    /* DB1_HIGH;      */
    /* pulseEnable(); */
    /* clearbytes();  */

    // Set character
    /* RS_HIGH;       */
    /* DB6_HIGH;      */
    /* DB3_HIGH;      */
    /* pulseEnable(); */
    /* clearbytes();  */
    /* RS_LOW;        */
}
