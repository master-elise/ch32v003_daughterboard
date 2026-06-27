#ifndef SWIO_H
#define SWIO_H
#include <avr/io.h>
#include <util/delay.h>

// SWIO on PD2
#define SWIO_DDR  DDRD
#define SWIO_PORT PORTD
#define SWIO_PIN  PIND
#define SWIO_BIT  2    // D0

void swio_init(void);
void swio_write_reg(uint8_t addr, uint32_t val);
uint32_t swio_read_reg(uint8_t addr);

#endif
