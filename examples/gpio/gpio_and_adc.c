#include "ch32fun.h"
#include <stdio.h>

#define N 10
#define k2000

int main() // PD0 PC7 PC5 PC3 PC1 PD3 PD2 PC6 PC4 PC2
{char port[N]={3,2,2,2,2,3,3,2,2,2};
 char pin[N]= {0,7,5,3,1,3,2,6,4,2};
 int k,n;
#ifdef k2000
 int dir;
#endif
 SystemInit();
 funGpioInitAll();
 funAnalogInit();

// see ../../misc/attic/hardware_header_all_combined: register = 16*port+pin
 for (k=0;k<N;k++) funPinMode( 16*port[k]+pin[k], GPIO_CFGLR_OUT_50Mhz_PP);
 for (k=0;k<N;k++) funDigitalWrite( 16*port[k]+pin[k], FUN_LOW);
      
 n=0;
#ifdef k2000
 dir=1;
#endif
 while(1)
  {funDigitalWrite( 16*port[n]+pin[n], FUN_LOW );
#ifndef k2000
   n=(n+1)%N;
#else
   if (dir==1) n=(n+1);
   else        n=(n-1);
   if ((n==0)||(n==N-1)) dir=1-dir;
#endif
   funDigitalWrite( 16*port[(n)]+pin[(n)], FUN_HIGH );
   Delay_Ms(80);
   printf( "Hello World\r\n");
  }
}
