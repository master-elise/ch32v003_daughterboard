MCU          = atmega32u4
ARCH         = AVR8
BOARD        = USER
F_CPU        = 16000000
F_USB        = 16000000
OPTIMIZATION = 1
TARGET       = main
LUFA_PATH    = lufa-ftdi/lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -Ilufa-ftdi/config/ -Ilufa-ftdi -Wall
#CC_FLAGS     += -Werror
LD_FLAGS     =
SRC          = \
	lufa-ftdi/main.c \
	lufa-ftdi/ftdi.c \
        lufa-ftdi/reset.c \
        uart.c \
        swio.c \
	$(LUFA_SRC_USB)

# Default target
deftarget: $(TARGET).hex size
.PHONY: deftarget

DEVICE = /dev/serial/by-id/usb-LUFA_FT232_*

# Misc helpers

# Open terminal on USB port
term:
	terminal.py $(DEVICE)
.PHONY: term

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_core.mk
$(LUFA_PATH)/Build/lufa_core.mk:
	@echo 'Did you forget "git submodule init; git submodule update"?'; false

# Remove some stuff from BASE_CC_FLAGS that the LUFA core put in there.
BASE_CC_FLAGS := $(filter-out -fno-inline-small-functions,$(BASE_CC_FLAGS))

flash: main.hex
	avrdude -c avr109 -b57600 -D -p atmega32u4 -P /dev/ttyACM0 -e -U flash:w:main.elf

test:
	../ch32fun/minichlink/minichlink -c /dev/ttyUSB3 -C ardulink -i

write:
	../ch32fun/minichlink/minichlink -c /dev/ttyUSB3 -C ardulink -w ../ch32fun/examples/gpio_and_adc/gpio_and_adc.bin flash -b

