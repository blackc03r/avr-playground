#include <avr/io.h>
#include <stdio.h>

// This code comes from https://arduino.stackexchange.com/a/29399 . I don't know
// where it comes from originally, but it works, so who cares!
static int uart_putchar(char c, FILE *stream)
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

    static FILE uart0fd;
    fdev_setup_stream((&uart0fd), uart_putchar, NULL,  _FDEV_SETUP_WRITE);
    stdout = &uart0fd;
}

