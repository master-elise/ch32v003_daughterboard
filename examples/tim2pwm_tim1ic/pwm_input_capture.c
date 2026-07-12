/* Small example showing how to capture timer values on gpio edges */

#include "ch32fun.h"
#include <stdio.h>

volatile uint32_t captureVal=0;

#define VMAX (100*100)

void TIM1_CC_IRQHandler(void) __attribute__((interrupt));
void TIM1_CC_IRQHandler(void)
{
  if(TIM1->INTFR & TIM_CC1IF)       // capture
    {captureVal = 0x00010000 | TIM1->CH1CVR; // capture value
     if(TIM1->INTFR & TIM_CC1OF)    // overflow
       {TIM1->INTFR = ~(TIM_CC1OF); // cleared by writing 0
        printf("OF1\n");
       }
    }
  else if (TIM1->INTFR & TIM_CC2IF)
    {captureVal = TIM1->CH2CVR;     // capture value
     if(TIM1->INTFR & TIM_CC2OF)    // overflow
       {TIM1->INTFR = ~(TIM_CC2OF); // cleared by writing 0
        printf("OF0\n");
       }
    }
  else
    printf("badtrigger\n");
}

// when set PWM outputs are held HIGH by default and pulled LOW
// when zero PWM outputs are held LOW by default and pulled HIGH
#define TIM2_DEFAULT 0xff
//#define TIM2_DEFAULT 0x00

void t2pwm_init( void )
{GPIOD->CFGLR &= ~(0xf<<(4*4)); // PD4 is T2CH1, 10MHz Output alt func, push-pull
 GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*4);
 GPIOD->CFGLR &= ~(0xf<<(4*3)); // PD3 is T2CH2, 10MHz Output alt func, push-pull
 GPIOD->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP_AF)<<(4*3);
 // SMCFGR: default clk input is CK_INT
 TIM2->PSC = 47;     // 1 MHz TIM2 clock prescaler divider 
 TIM2->ATRLR = VMAX; // set PWM total cycle width
	
 // for channel 1 and 2, let CCxS stay 00 (output), set OCxM to 110 (PWM I)
 // enabling preload causes the new pulse width in compare capture register only to come into effect when UG bit in SWEVGR is set (= initiate update) (auto-clears)
 TIM2->CHCTLR1 |= TIM_OC1M_2 | TIM_OC1M_1 | TIM_OC1PE | TIM_OC2M_2 | TIM_OC2M_1 | TIM_OC2PE;

 // CTLR1: default is up, events generated, edge align
 // enable auto-reload of preload
 TIM2->CTLR1 |= TIM_ARPE;

 // Enable Channel outputs, set default state (based on TIM2_DEFAULT)
 TIM2->CCER |= TIM_CC1E | (TIM_CC1P & TIM2_DEFAULT);
 TIM2->CCER |= TIM_CC2E | (TIM_CC2P & TIM2_DEFAULT);

 TIM2->SWEVGR |= TIM_UG; // initialize counter
 TIM2->CTLR1 |= TIM_CEN; // Enable TIM2
}

void t2pwm_setpw(uint8_t chl, uint16_t width)
{switch(chl&3)
  {case 0: TIM2->CH1CVR = width; break;
   case 1: TIM2->CH2CVR = width; break;
  }
  //TIM2->SWEVGR |= TIM_UG; // load new value in compare capture register
}

int main()
{uint32_t count = 0,countval = 100;
 SystemInit();
 Delay_Ms(100);
 RCC->APB2PCENR |= RCC_IOPDEN | RCC_TIM1EN | RCC_APB2Periph_GPIOD; // must be |= to avoid disabling UART
 RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

 RCC->APB2PRSTR |= RCC_TIM1RST;
 RCC->APB2PRSTR &= ~RCC_APB2Periph_TIM1;
 RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
 RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2; // Reset TIM2 to init all regs

 //  D2 Capture Input(T1CH1), default analog input
 GPIOD->CFGLR |= (((GPIO_Speed_In | GPIO_CNF_IN_FLOATING) << (4 * 1)) | 
                 ((GPIO_Speed_In | GPIO_CNF_IN_FLOATING) << (4 * 2)));  // must be |= to avoid disabling UART

 TIM1->ATRLR = 0xffff;
 TIM1->PSC = 47; // 48MHz/(47+1) -> 1µs resolution
 TIM1->CHCTLR1 = TIM_CC1S_0 | TIM_CC2S_1;
 TIM1->CCER = TIM_CC1E|TIM_CC2E; // |TIM_CC2P; // CC2P = rising (0) or falling (1) edge
 TIM1->CTLR1 = TIM_CEN;

 t2pwm_init();
 t2pwm_setpw(0, countval);	// CH1 PD3
 t2pwm_setpw(1, countval);	// CH2 PD4

 NVIC_EnableIRQ(TIM1_CC_IRQn);
 TIM1->DMAINTENR = TIM_CC1IE|TIM_CC2IE;

 while (1)
   {if (captureVal != 0) 
      {printf("%lu %lu %lu\n",(long)countval, (long)(captureVal>>16),(long)(captureVal&0xFFFF));
       captureVal=0;
       count++;
      }
    if (count==5)	  
      {countval += (VMAX/100);
       countval %= VMAX;
       if (countval == 0) countval=100; // must trigger IC
       t2pwm_setpw(0, countval);	// CH1 PD3
       t2pwm_setpw(1, countval);	// CH2 PD4
       count=0;
      }
   }
}

