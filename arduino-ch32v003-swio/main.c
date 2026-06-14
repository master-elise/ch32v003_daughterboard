#include <stdio.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <stdint.h>
#include "uart.h"
#include "swio.h"
#include <ctype.h>
#include <util/delay.h>

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <LUFA/Drivers/USB/USB.h>

#include "ftdi.h"


#define TARGET_POWER_PORT  PORTD
#define TARGET_POWER_DDR   DDRD
#define TARGET_POWER_BIT   2

void target_power(int x) {
    if (x)
        TARGET_POWER_PORT |= _BV(TARGET_POWER_BIT);
    else
        TARGET_POWER_PORT &= ~_BV(TARGET_POWER_BIT);
}

#define PROTOCOL_START     '!'
#define PROTOCOL_ACK       '+'
#define PROTOCOL_TEST      '?'
#define PROTOCOL_POWER_ON  'p'
#define PROTOCOL_POWER_OFF 'P'
#define PROTOCOL_WRITE_REG 'w'
#define PROTOCOL_READ_REG  'r'


int main(void) {
    uint8_t reg;
    uint32_t val;

    MCUSR &= ~(1 << WDRF);
    wdt_disable();


    // Make the target reset pin an output.
    TARGET_POWER_DDR |= _BV(TARGET_POWER_BIT);

    uart_init();
    swio_init();

DDRE |= 1<<6;
for (int k=0;k<3;k++)
  {PORTE&=~(1<<6);
   _delay_us(100000);
   PORTE|=(1<<6);
   _delay_us(100000);
  }

    //fputc(PROTOCOL_START, uart);
    printf("%c",PROTOCOL_START);
    while (1) {
        switch (getchar()) { // fgetc(uart)) {
            case PROTOCOL_TEST:
                // fputc(PROTOCOL_ACK, uart);
                printf("%c",PROTOCOL_ACK);
                break;
            case PROTOCOL_POWER_ON:
                target_power(1);
                // fputc(PROTOCOL_ACK, uart);
                printf("%c",PROTOCOL_ACK);
                break;
            case PROTOCOL_POWER_OFF:
                target_power(0);
                // fputc(PROTOCOL_ACK, uart);
                printf("%c",PROTOCOL_ACK);
                break;
            case PROTOCOL_WRITE_REG:
                fread(&reg, sizeof(uint8_t), 1, uart);
                fread(&val, sizeof(uint32_t), 1, uart);
                swio_write_reg(reg, val);
                //printf("%d",reg);   // tmp
                //printf("%lx",val); // tmp
                //fputc(PROTOCOL_ACK, uart);
                printf("%c",PROTOCOL_ACK);
                break;
            case PROTOCOL_READ_REG:
                fread(&reg, sizeof(uint8_t), 1, uart);
                val = swio_read_reg(reg);
//                fwrite(&val, sizeof(uint32_t), 1, uart);
                putchar((val)&0xff);
                putchar((val>>8)&0xff);
                putchar((val>>16)&0xff);
                putchar((val>>24)&0xff);
                break;
        }
    }
}
