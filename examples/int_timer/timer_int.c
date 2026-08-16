#include "ch32fun.h"

volatile int val;

void init_timer() {
    // TIMER
    RCC->APB2PCENR |= RCC_APB2Periph_TIM1;
    TIM1->CTLR1 |= TIM_CounterMode_Up | TIM_CKD_DIV1; // see RM p.96
    TIM1->CTLR2 = TIM_MMS_1;
    TIM1->ATRLR = 5000;      // 1 Hz
    TIM1->PSC = 4800-1;      // prescale = 48 MHz/4800 = 0.1 ms
    TIM1->RPTCR = 0;
    TIM1->SWEVGR = TIM_PSCReloadMode_Immediate;

    NVIC_EnableIRQ(TIM1_UP_IRQn);
    TIM1->INTFR = ~TIM_FLAG_Update;
    TIM1->DMAINTENR |= TIM_IT_Update;
    TIM1->CTLR1 |= TIM_CEN;
}

void TIM1_UP_IRQHandler(void) __attribute__((interrupt));
void TIM1_UP_IRQHandler() {
    if(TIM1->INTFR & TIM_FLAG_Update) {
        TIM1->INTFR = ~TIM_FLAG_Update;
    }
    if (val==1) funDigitalWrite( PC2, FUN_HIGH );
    else funDigitalWrite( PC2, FUN_LOW );
    val=1-val;
}

int main() {
    SystemInit();
    val=0;
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
    funPinMode( PC2, GPIO_CFGLR_OUT_10Mhz_PP );  // LED on PC1
    init_timer();
    while(1) {asm volatile( "nop" );}
}
