// https://pallavaggarwal.in/2023/11/22/ch32v003-programming-how-to-use-spi/

#include "ch32fun.h"
#include <stdio.h>

#define CH32V003_SPI_SPEED_HZ	1000000

u8 CS_PIN = PC3;

#define cs_lo funDigitalWrite(CS_PIN, 0);
#define cs_hi funDigitalWrite(CS_PIN, 1);

#define CH32V003_SPI_DIRECTION_2LINE_TXRX
#define CH32V003_SPI_CLK_MODE_POL0_PHA0
#define CH32V003_SPI_NSS_SOFTWARE_ANY_MANUAL

#include "../../extralibs/ch32v003_SPI.h"

void binaire(char c,char *o)
{int k;
 for (k=7;k>=0;k--) o[7-k]=((c>>k)&0x01)+'0'; // 0 ou 1
}

void SPI_Configure() {
	SPI1->CTLR1 = 0;                    // reset control register

        // Enable GPIO Port C and SPI peripheral
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOC | RCC_APB2Periph_SPI1; 

	GPIOC->CFGLR &= ~(0xf << (4*5));    // PC5 is SCLK
	GPIOC->CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_OUT_PP_AF) << (4*5);

	GPIOC->CFGLR &= ~(0xf << (4*6));    // PC6 is MOSI
	GPIOC->CFGLR |= (GPIO_Speed_50MHz | GPIO_CNF_OUT_PP_AF) << (4*6);

	GPIOC->CFGLR &= ~(0xf << (4 * 7));  // PC7 is MISO
	GPIOC->CFGLR |= GPIO_CNF_IN_FLOATING << (4 * 7);

	SPI1->CTLR1 |= SPI_CPHA_1Edge | SPI_CPOL_Low
				| SPI_Mode_Master| SPI_BaudRatePrescaler_256
				| SPI_NSS_Soft | SPI_DataSize_8b;
	
	SPI1->CTLR1 |= SPI_Direction_2Lines_FullDuplex;

	SPI1->CTLR1 |= CTLR1_SPE_Set;       // Enable SPI Port
}

int main()
{ unsigned char ch,cl; // DOIVENT etre unsigned (sinon erreur lors de la concatenation)
  int k; int32_t temperature;
  char buffer[40];

  SystemInit();
  Delay_Ms(50);
  funGpioInitAll();
  printf("LM74\r\n");
  
  funPinMode( CS_PIN, GPIO_CFGLR_OUT_50Mhz_PP);
  funDigitalWrite( CS_PIN, FUN_HIGH);
  SPI_Configure();
  buffer[16]=0;              // end of string
  
  while (1) {
    cs_lo;                   // active le capteur
    ch=SPI_transfer_8(0x55); binaire(ch,buffer);
    cl=SPI_transfer_8(0x00); binaire(cl,&buffer[8]);
    for (k=0;k<2;k++) {SPI_transfer_8(0x00);}
    cs_hi;
    printf("%s ",buffer); 
    printf("%04x = ",ch*256+cl);
    temperature=(((int32_t)(ch*256+cl)>>3)*625)/10;
    printf("%03ld.%03ld\r\n",temperature/1000,temperature-(temperature/1000)*1000);
    Delay_Ms(100);
  }
  return(0);
}
