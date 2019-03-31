#include <avr/io.h>
#include <util/delay.h>

static void uart_putchar(char c)
{
    if (c == '\n') {
        uart_putchar('\r');
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

static void uart_putstr(const char *s)
{
    while (*s != '\0') {
        uart_putchar(*s);
        s++;
    }
}

// function to initialize UART
static void uart_init(unsigned long int baud)
{
    unsigned int baudrate = F_CPU/(baud*16UL)-1;
    UBRR0H = (baudrate>>8);             // shift the register right by 8 bits
    UBRR0L = baudrate;                  // set baud rate
    UCSR0B|= (1<<TXEN0)|(1<<RXEN0);     // enable receiver and transmitter
}

/* Outputs "Hello World!" to the serial console every second.
 */
int main (void)
{
    uart_init(9600);
    while (1) {
        uart_putstr("Hello World!\n");
        _delay_ms(1000);
    }
    return 0;
}
