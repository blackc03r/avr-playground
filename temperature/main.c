#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

/* Outputs temperature to serial console.
 *
 * - TMP36, powered by 5V, grounded, connected to A0.
 *
 * Every second, prints a number to the console. That number is related to the temperature of the
 * termometer. The TMP36 outputs 750mA at 25 degrees Celsius. Considering that our ADC range is
 * 0-1024, that gives us 4.88 mA per step.
 *
 * This code is the result of a *lot* of messing around, reading datasheets and tutorials. But
 * mostly messing around... But now it seems to work!
 *
 */

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/* Outputs "Hello World!" to the serial console every second.
 */
// This code comes from https://arduino.stackexchange.com/a/29399 . I don't know
// where it comes from originally, but it works, so who cares!
int uart_putchar(char c, FILE *stream)
{
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

// function to initialize UART
void uart_init(unsigned long int baud)
{
    unsigned int baudrate = F_CPU/(baud*16UL)-1;
    UBRR0H = (baudrate>>8);             // shift the register right by 8 bits
    UBRR0L = baudrate;                  // set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0);     // enable receiver and transmitter
    UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);   // 8bit data format

    //// with static FILE variable:
    // static FILE uart0fd;
    // fdev_setup_stream((&uart0fd), uart_putchar, NULL,  _FDEV_SETUP_WRITE);
    // stdout = &uart0fd;

    //// or directly by using dynamically allocated FILE struct:
    stdout = fdevopen(uart_putchar, NULL); // uses malloc/calloc, it might be inconvenient
}

int main()
{
    uint8_t high, low;
    int val;

    uart_init(9600);
    /* DIDR0 = 0; // Disable digital input on all ADC ports                            */
    /* PRR &= ~_BV(PRADC); // ADC turned on                                            */
    /* ADMUX = 0x60; //AVcc, right adjusted, ADC0 pin                                  */
    /* ADCSRA = 0xcF; //ADC Enabled, no auto trigger, Iterrupt enabled, 128 prescaller */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADMUX |= (1 << REFS0);
    /* ADCSRB &= ~((1 << ADTS0) | (1 << ADTS1)| (1 << ADTS2)); */
    ADCSRA |= (1 << ADEN);

    while (1) {
        cbi(PRR, PRADC);
        sbi(ADCSRA, ADSC);
        while (bit_is_set(ADCSRA, ADSC));
        low = ADCL;
        high = ADCH;
        val = (high << 8) | low;
        printf("Hello %d\n", val);
        _delay_ms(1000);
    }
}
