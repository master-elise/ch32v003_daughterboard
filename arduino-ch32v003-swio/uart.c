#include "uart.h"
#ifdef F_USB
#include <avr/power.h>
#include "ftdi.h"
#endif

#ifndef F_USB
int _uart_putchar(char c, FILE *unused);
int _uart_getchar(FILE *unused);
FILE uart_file = FDEV_SETUP_STREAM(_uart_putchar, _uart_getchar, _FDEV_SETUP_READ | _FDEV_SETUP_WRITE);
FILE *uart = &uart_file;
#else
FILE *uart;
#endif

void uart_init() {
#ifndef F_USB
    // Enable 2X clock.
    UCSR0A |= _BV(U2X0);

    // Set the baud rate divider.
    // 16M / 8 / (16 + 1) = 117647 bps (2% error)
    UBRR0L = 16;

    // Enable TX.
    UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
#else
    /* Divide 8MHz clock by 1 */
    clock_prescale_set(clock_div_1);

    /* Initialize USB */
    USB_Init();

    /* Initialize USB serial and USB interrupts */
    ftdi_init(FTDI_STDIO | FTDI_BLOCKING);

    /* Ready to go */
    uart=stdout;  // needed to return to the original function fputc(val, uart); instead of printf();
    sei();        // stdout = ftdi_stream (tested by printing pointer values) so uart is assigned FILE *ftdi_stream = &_ftdi_stream; in lufa_ftdi/ftdi.c 
#endif            
}                

#ifndef F_USB
int _uart_putchar(char c, FILE *uart) {
    // Wait for the UART to become ready.
    while (!(UCSR0A & _BV(UDRE0)))
        ;

    // Send.
    UDR0 = c;

    return 0;
}

int _uart_getchar(FILE *unused) {
    (void)*unused;

    // Wait for a character.
    while (!(UCSR0A & _BV(RXC0)))
        ;

    return UDR0;
}
#endif
