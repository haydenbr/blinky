TARGET = blinky
SRCS = ./src/blinky.c

LINKER_SCRIPT = ./flash/STM32F429ZITX_FLASH.ld

# Generate debug info
DEBUG = 1

# Choose processor
CDEFS  = -DSTM32F429xx
# Enable FPU
#CDEFS += -D__VFP_FP__

include ./armf4.mk