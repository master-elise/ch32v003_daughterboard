#include "ch32fun.h"

#define LED_PIN PC0

static void tim2_init(void)
{RCC->APB1PCENR |= RCC_APB1Periph_TIM2; // TIM2 clock
 RCC->APB1PRSTR |= RCC_APB1Periph_TIM2; // TIM2 reset
 RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;

 TIM2->PSC    = 4799;    // freq=48 MHz/(4799+1)=10 kHz
 TIM2->ATRLR  = 9;       // ARR = 9 => ovf=10 kHz/10=1 kHz=1 ms
 TIM2->CTLR1  = TIM_CEN; // counter enable
 TIM2->SWEVGR = TIM_UG;  // prescaler load 
 TIM2->INTFR  = 0;
}

int main(void)
{SystemInit();
 funPinMode(LED_PIN, GPIO_CFGLR_OUT_10Mhz_PP);
 tim2_init();
 while (1)
  {if (TIM2->INTFR & TIM_UIF) // tim ovf flah
      {TIM2->INTFR &= ~TIM_UIF;
       funDigitalWrite(LED_PIN, !funDigitalRead(LED_PIN));
      }
  }
}
