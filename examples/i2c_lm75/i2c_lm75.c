/******************************************************************************
* Basic Example of using lib_i2c on the CH32V003 Microcontroller
*
*
* Demo Version 3.3    16 Aug 2025 
* See GitHub Repo for more information: 
* https://github.com/ADBeta/CH32V00x_lib_i2c
*
*
* Notes:
* When selecting a Clock Speed (on the CH32V003), Values between 10KHz and 1MHz
* work reliably and consistently. Values outside of this range can be sketchy
* 
* Alternatively, use one of the pre-defined Clock Speeds:
* I2C_CLK_10KHZ    I2C_CLK_50KHZ    I2C_CLK_100KHZ    I2C_CLK_400KHZ
* I2C_CLK_500KHZ   I2C_CLK_600KHZ   I2C_CLK_750KHZ    I2C_CLK_1MHZ
*
*
* To select the HW Pins used by the I2C Device is done via defining one of the
* following options - BEFORE THE HEADER IS INCLUDED.
* WARN: The best place to do this is in funconfig.h
*
* #define I2C_PINOUT_DEFAULT       SCL = PC2    SDA = PC1
* #define I2C_PINOUT_ALT_1         SCL = PD1    SDA = PD0
* #define I2C_PINOUT_ALT_2         SCL = PC5    SDA = PC6
*
*
* Read lib_i2c.h for documentation on each function and how to use them
*
* Released under the MIT Licence
* Copyright ADBeta (c) 2024 - 2025
******************************************************************************/
#include "ch32fun.h"
#include "lib_i2c.h"

#include <stdio.h>


// I2C Scan Callback example function. Prints the address which responded
void i2c_scan_callback(const uint8_t addr)
{
	printf("Address: 0x%02X Responded.\n", addr);
}


int main() 
{
  uint8_t temperature[3];   
	SystemInit();
	i2c_device_t dev = {
		.clkr = I2C_CLK_50KHZ, // * Clock Speed
		.type = I2C_ADDR_7BIT,  // * Address Type (7bit or 10bit)
		.addr = 0x4F,           // * Address Value
		.regb = 2,              // * Register Bytes (1, 2, 3 & 4 bytes supported)
		.tout = 2000,           // * Timeout Count
	};

	// Initialise the I2C Interface
	if(i2c_init(&dev) != I2C_OK) printf("Failed to init the I2C Bus\n");

	// Initialising I2C causes the pins to transition from LOW to HIGH.
	// Wait 100ms to allow the I2C Device to timeout and ignore the transition.
	// Otherwise, an extra 1-bit will be added to the next transmission
	Delay_Ms(250);

	printf("----Scanning I2C Bus for Devices----\n");
	i2c_scan(i2c_scan_callback);
	printf("----Done Scanning----\n\n");

        temperature[0]=0;

        i2c_err_t i2c_stat;
//        i2c_stat = i2c_write_reg(&dev, 0x01, temperature, 1);
        // i2c_stat = i2c_write_raw(&dev, temperature, 1);
//        if(i2c_stat != I2C_OK) { printf("Write error\n"); }

        while (1)
         {Delay_Ms(100);
          i2c_stat = i2c_read_reg(&dev, 0x00, temperature+1, 2);
//          i2c_stat = i2c_write_reg(&dev, 0x00, temperature, 1);
          // i2c_stat = i2c_read_raw(&dev, temperature+1, 2);
          if(i2c_stat != I2C_OK) printf("Read error ");
          // printf("%d %d\n",temperature[1],temperature[2]); // 9 bits
          // printf("%03d.%01d\n",temperature[1],temperature[2]); // 9 bits
          printf("%03d.%01d\n",temperature[1],(temperature[2]>>7)*5); // 9 bits
         }
}
