#include "ch32fun.h"

void EXTI7_0_IRQHandler( void ) INTERRUPT_DECORATOR;
void EXTI7_0_IRQHandler( void ) 
{
 NVIC_DisableIRQ( EXTI7_0_IRQn );
 funDigitalWrite( PC1, FUN_HIGH );
 Delay_Ms(2000);
 funDigitalWrite( PC1, FUN_LOW );
 EXTI->INTFR = EXTI_Line0; 
 NVIC_EnableIRQ( EXTI7_0_IRQn );
}

int main()
{SystemInit();
 RCC->APB2PCENR = RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO;
 funPinMode( PC0, GPIO_CFGLR_IN_FLOAT );      // button on PC0
 funPinMode( PC1, GPIO_CFGLR_OUT_10Mhz_PP );  // LED on PC1
 funPinMode( PC2, GPIO_CFGLR_OUT_10Mhz_PP );  // blinking LED on PC1
 AFIO->EXTICR = AFIO_EXTICR_EXTI0_PC;         // PORT and PIN number as interrupt
 EXTI->INTENR = EXTI_INTENR_MR0;              // enable EXT0
 EXTI->RTENR = EXTI_FTENR_TR0;                // rising edge trigger (FTENR for falling)
 NVIC_EnableIRQ( EXTI7_0_IRQn );
 while(1)
 {asm volatile( "nop" );
  funDigitalWrite( PC2, FUN_HIGH ); Delay_Ms(280); funDigitalWrite( PC2, FUN_LOW ); Delay_Ms(280);
 }
}
