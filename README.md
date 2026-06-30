<img src="ch32v003_daughterboard.png">

See https://www.sameskydevices.com/product/resource/3dmodel/ptn091-v05115k1b found at https://eu.mouser.com/ProductDetail/Same-Sky/PTN091-V05115K1B?qs=IKkN%2F947nfCwGg20YudwyA%3D%3D

See https://www.olimex.com/Products/RISC-V/WCH-CH32V003EVT/

## Using https://github.com/cnlohr/ch32fun

The proposed toolchain is compiled with https://github.com/master-elise/summon-arm-toolchain and its ``summon-riscv-toolchain`` script. The resulting toolchain will be located in ``$HOME/svt`` so that ``$HOME/svt/bin`` must be added to the ``$PATH`` to be used.

The ``ch32fun.mk`` from https://github.com/cnlohr/ch32fun is updated by replacing 
```
PREFIX_DEFAULT:=riscv64-elf
```
with
```
PREFIX_DEFAULT:=riscv32-elf
```
and removing 
```
ifneq ($(shell $(WHICH) riscv64-unknown-elf-gcc 2>$(NULLDEV)),)
        PREFIX_DEFAULT:=riscv64-unknown-elf
else ifneq ($(shell $(WHICH) riscv-none-elf-gcc 2>$(NULLDEV)),)
        PREFIX_DEFAULT:=riscv-none-elf
else ifneq ($(shell $(WHICH) riscv64-linux-gnu-gcc 2>$(NULLDEV)),)
        PREFIX_DEFAULT:=riscv64-linux-gnu
else ifeq ($(OS_NAME),netbsd)
        # assume using pkgsrc/cross/riscv64-none-elf-gcc and
        # pkgsrc/cross/riscv64-none-elf-binutils
        PKGSRC_LOCALBASE ?= /usr/pkg
        ifneq ($(wildcard $(PKGSRC_LOCALBASE)/cross-riscv64-none-elf/bin/riscv64-none-elf-gcc),)
                PREFIX_DEFAULT := $(PKGSRC_LOCALBASE)/cross-riscv64-none-elf/bin/riscv64-none-elf
        endif
endif
```
which would otherwise overwrite our change.

To check, goto
```
cd ch32fun/examples/gpio_and_adc
make
```
must conclude with
```
riscv32-elf-objdump -S gpio_and_adc.elf > gpio_and_adc.lst
riscv32-elf-objcopy -R .storage  -O binary gpio_and_adc.elf gpio_and_adc.bin
riscv32-elf-objcopy -j .storage -O binary gpio_and_adc.elf gpio_and_adc_ext.bin
riscv32-elf-objcopy -O ihex gpio_and_adc.elf gpio_and_adc.hex
../../ch32fun//../minichlink/minichlink -w gpio_and_adc.bin flash -b
```
and ``minichlink`` might or might not fail depending on the programmer available on the computer. When using the Atmega32U4-based programmer found in ``arduino-ch32v003-swio``, replace the programming command with
```
../../minichlink/minichlink -c /dev/ttyUSB0 -C ardulink -w gpio_and_adc.bin flash -b
```

## Example of a GDB session using the ardulink interface

Terminal 1:
```
ch32fun/examples/gpio_and_adc$ ../../minichlink/minichlink -c /dev/ttyUSB0 -C ardulink -G
minichlink version - 8e9d331abce4f0aff5fc4f1c162844bff76fbf28
Opening serial port /dev/ttyUSB0 at 115200 baud.
Ardulink: synced.
Ardulink: target power 1
Interface Setup
Detected CH32V003
Flash Storage: 16 kB
Part UUID: 9b-77-ab-cd-04-72-bd-2e
Part Type: 00-30-05-10
Read protection: disabled
gdbserver running on port 3333
GDBServer Running
```

Terminal 2:
```
ch32fun/examples/gpio_and_adc$ gdb-multiarch -q gpio_and_adc.elf
(gdb) target remote localhost:3333
Remote debugging using localhost:3333
0x00000862 in DelaySysTick (n=6000000) at ../../ch32fun/ch32fun.c:1884
1884            while( ((int32_t)( SysTick->CNT - targend )) < 0 );
(gdb) list main
...
(gdb) list
...
(gdb) list
...
60                      printf( "1: %d %d %d %d\n",
(gdb) break 60
Breakpoint 1 at 0x7d8: file ch32fun/examples/gpio_and_adc/gpio_and_adc.c, line 60.
(gdb) continue
Continuing.

Breakpoint 1, main () at ch32fun/examples/gpio_and_adc/gpio_and_adc.c:60
60                      printf( "1: %d %d %d %d\n",
(gdb) bt
#0  main () at ch32fun/examples/gpio_and_adc/gpio_and_adc.c:60
(gdb) print $sp
$1 = (void *) 0x200007e8
```

<img src="IMG_20260630_152816_968.jpg">
