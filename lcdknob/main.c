#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "../common/util.h"

/* A LCD screen with its content being controlled by buttons and a knob.
 */

#define RS_HIGH sbi(PORTB, PORTB3)
#define RS_LOW cbi(PORTB, PORTB3)
#define RW_HIGH sbi(PORTB, PORTB4)
#define RW_LOW cbi(PORTB, PORTB4)
#define E_HIGH sbi(PORTB, PORTB2)
#define E_LOW cbi(PORTB, PORTB2)
#define DB0_HIGH sbi(PORTB, PORTB1)
#define DB0_LOW cbi(PORTB, PORTB1)
#define DB1_HIGH sbi(PORTB, PORTB0)
#define DB1_LOW cbi(PORTB, PORTB0)
#define DB2_HIGH sbi(PORTD, PORTD7)
#define DB2_LOW cbi(PORTD, PORTD7)
#define DB3_HIGH sbi(PORTD, PORTD6)
#define DB3_LOW cbi(PORTD, PORTD6)
#define DB4_HIGH sbi(PORTD, PORTD5)
#define DB4_LOW cbi(PORTD, PORTD5)
#define DB5_HIGH sbi(PORTD, PORTD4)
#define DB5_LOW cbi(PORTD, PORTD4)
#define DB6_HIGH sbi(PORTD, PORTD3)
#define DB6_LOW cbi(PORTD, PORTD3)
#define DB7_HIGH sbi(PORTD, PORTD2)
#define DB7_LOW cbi(PORTD, PORTD2)

void clearbytes()
{
    DB0_LOW;
    DB1_LOW;
    DB2_LOW;
    DB3_LOW;
    DB4_LOW;
    DB5_LOW;
    DB6_LOW;
    DB7_LOW;
}

void debugLED(bool on)
{
    if (on) {
        sbi(PORTB, PORTB5);
    } else {
        cbi(PORTB, PORTB5);
    }
}

void pulseEnable()
{
    E_LOW;
    _delay_us(1);
    E_HIGH;
    _delay_us(1);
    E_LOW;
    _delay_us(1);
}

// Returns whether the BF flag was ON at least for one cycle
void wait_for_bf(bool expect)
{
    RW_HIGH;
    cbi(DDRD, DDD2);
    if (expect) {
        while (!(PIND & _BV(PIND2))) {
            pulseEnable();
        }
    } else {
        while (PIND & _BV(PIND2)) {
            pulseEnable();
        }
    }
    sbi(DDRD, DDD2);
    RW_LOW;
}

void pulseEnableAndWaitBF()
{
    pulseEnable();
    wait_for_bf(true);
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

/* void write4bits(uint8_t value)                            */
/* {                                                         */

/*     for (int i = 0; i < 4; i++) {                         */
/*         digitalWrite(_data_pins[i], (value >> i) & 0x01); */
/*     }                                                     */

/*     pulseEnable();                                        */
/* }                                                         */

/* void write8bits(uint8_t value)                            */
/* {                                                         */
/*     for (int i = 0; i < 8; i++) {                         */
/*         digitalWrite(_data_pins[i], (value >> i) & 0x01); */
/*     }                                                     */

/*     pulseEnable();                                        */
/* }                                                         */

/* void send(uint8_t value, uint8_t mode)                     */
/* {                                                          */
/*     digitalWrite(_rs_pin, mode);                           */

/*     // if there is a RW pin indicated, set it low to Write */
/*     if (_rw_pin != 255) {                                  */
/*         digitalWrite(_rw_pin, LOW);                        */
/*     }                                                      */

/*     if (_displayfunction & LCD_8BITMODE) {                 */
/*         write8bits(value);                                 */
/*     } else {                                               */
/*         write4bits(value>>4);                              */
/*         write4bits(value);                                 */
/*     }                                                      */
/* }                                                          */


void LEDDance() {
    _delay_ms(1000);
    DB7_HIGH;
    _delay_ms(1000);
    DB7_LOW;
    _delay_ms(1000);
    DB6_HIGH;
    _delay_ms(1000);
    DB6_LOW;
    _delay_ms(1000);
    DB5_HIGH;
    _delay_ms(1000);
    DB5_LOW;
    _delay_ms(1000);
    DB4_HIGH;
    _delay_ms(1000);
    DB4_LOW;
    _delay_ms(1000);
    DB3_HIGH;
    _delay_ms(1000);
    DB3_LOW;
    _delay_ms(1000);
    DB2_HIGH;
    _delay_ms(1000);
    DB2_LOW;
    _delay_ms(1000);
    DB1_HIGH;
    _delay_ms(1000);
    DB1_LOW;
    _delay_ms(1000);
    DB0_HIGH;
    _delay_ms(1000);
    DB0_LOW;
    _delay_ms(1000);
    E_HIGH;
    _delay_ms(1000);
    E_LOW;
    _delay_ms(1000);
    RS_HIGH;
    _delay_ms(1000);
    RS_LOW;
    _delay_ms(1000);
    RW_HIGH;
    _delay_ms(1000);
    RW_LOW;
}

int main()
{
    DDRB |= _BV(DDB5); // Debug LED

    debugLED(true);
    wait_for_bf(true);
    debugLED(false);

    DDRB |= _BV(DDB3); // LED RS
    DDRB |= _BV(DDB4); // LED RW
    DDRB |= _BV(DDB2); // LED E
    DDRB |= _BV(DDB1); // LED DB0
    DDRB |= _BV(DDB0); // LED DB1
    DDRD |= _BV(DDD7); // LED DB2
    DDRD |= _BV(DDD6); // LED DB3
    DDRD |= _BV(DDD5); // LED DB4
    DDRD |= _BV(DDD4); // LED DB5
    DDRD |= _BV(DDD3); // LED DB6
    DDRD |= _BV(DDD2); // LED DB7
    clearbytes();

    RS_LOW;
    E_LOW;

    wait_for_bf(false);

    // display clear
    /* DB0_HIGH;               */
    /* pulseEnableAndWaitBF(); */
    /* clearbytes();           */

    // function set
    /* DB5_HIGH;               */
    /* DB4_HIGH;               */
    /* pulseEnableAndWaitBF(); */
    /* clearbytes();           */

    /* _delay_ms(1000); */

    // Display mode
    DB3_HIGH;
    /* DB2_HIGH; */
    /* DB1_HIGH; */
    pulseEnableAndWaitBF();
    clearbytes();

    blink_forever(true);

    // entry mode
    DB2_HIGH;
    DB1_HIGH;
    pulseEnable();
    clearbytes();

    // Return home
    /* DB1_HIGH;      */
    /* pulseEnable(); */
    /* clearbytes();  */

    // Set character
    RS_HIGH;
    DB6_HIGH;
    DB3_HIGH;
    pulseEnable();
    clearbytes();
    RS_LOW;
}
