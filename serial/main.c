#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../common/uart.h"

/* Outputs "Hello World!" to the serial console every second.
 */
int main (void)
{
    uart_init(9600);
    while (1) {
        printf("Hello world!\n");
        _delay_ms(1000);
    }
    return 0;
}
