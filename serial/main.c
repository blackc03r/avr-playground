#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

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

int main (void)
{
    uart_init(9600);
    while (1) {
        printf("Hello world!\n");
        _delay_ms(1000);
    }
    return 0;
}
