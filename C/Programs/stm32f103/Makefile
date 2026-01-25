#Make file for Windows OS
#For Linux some changes need to be made.

#Toolchain Commands
CC=arm-none-eabi-gcc
OBJ_DUMP=arm-none-eabi-objdump.exe
OBJ_COPY=arm-none-eabi-objcopy
NM=arm-none-eabi-nm.exe
GDB=arm-none-eabi-gdb.exe
FLASH=st-flash --reset write
PROBE=st-info --probe

#Toolchain Paths
OPENOCD_DIR=../../../toolchain/xpack-openocd-0.12.0-3/openocd

#Openocd
#Configuration File
#Interface
OPENOCD_INTF_CFG_FILE=$(OPENOCD_DIR)/scripts/interface/stlink-v2.cfg
#target
OPENOCD_TGT_CFG_FILE=$(OPENOCD_DIR)/scripts/target/stm32f1x.cfg

#MCU Params
MACH=cortex-m3 #device
FLASH_ADDR=0x08000000 #flash address where the code needs to be loaded

#Directories
OBJ_DIR=Objects
INC_DIR=../../Device_Drivers/stm32f103/inc
SRC_DIR=../../Device_Drivers/stm32f103/src
UTIL_DIR=../../Device_Drivers/utilities
LOG_DIR=../Logs

#Files
MAIN=LED_toggle
ELF=final.elf
STARTUP=../../Device_Drivers/stm32f103/others/bp_startup
SYSCALLS=../../Device_Drivers/stm32f103/src/syscalls
LINK=../../Device_Drivers/stm32f103/others/bp_linker.ld
DRIVERS=stm32f103xx_gpio.o stm32f103xx_afio.o stm32f103xx_exti.o stm32f103xx_nvic.o stm32f103xx_usart.o stm32f103xx_rcc.o stm32f103xx_rtc.o stm32f103xx_spi.o stm32f103xx_adc.o stm32f103xx_i2c.o stm32f103xx_timer.o stm32f103xx_dma.o stm32f103xx_systick.o
OTHERS=stm32f103xx_init.o stm32f103xx_serial.o stm32f103xx_fault_handlers.o
UTILS=stm32f103xx_utilities.o
#DISPLAY=
#SENSORS=

#Flags
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -Wno-unused -O0 -I$(INC_DIR)
DEBUG_FLAG= -g3
#LDFLAGS= -mcpu=$(MACH) -mthumb --specs=nano.specs -T $(LINK) -Wl,-Map=logs/final.map
LDFLAGS= --specs=nano.specs -mcpu=$(MACH) -mthumb -T $(LINK) -Wl,-Map=$(LOG_DIR)final.map
#LDFLAGS= -mcpu=$(MACH) -mthumb -nostdlib -T $(LINK) -Wl,-Map=logs/final.map


all: compile load
compile: clean final.elf clean-obj
stm32f103xx_gpio.o:$(SRC_DIR)/stm32f103xx_gpio.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_afio.o:$(SRC_DIR)/stm32f103xx_afio.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_exti.o:$(SRC_DIR)/stm32f103xx_exti.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_nvic.o:$(SRC_DIR)/stm32f103xx_nvic.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_usart.o:$(SRC_DIR)/stm32f103xx_usart.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_rcc.o:$(SRC_DIR)/stm32f103xx_rcc.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_rtc.o:$(SRC_DIR)/stm32f103xx_rtc.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_spi.o:$(SRC_DIR)/stm32f103xx_spi.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_adc.o:$(SRC_DIR)/stm32f103xx_adc.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_i2c.o:$(SRC_DIR)/stm32f103xx_i2c.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_timer.o:$(SRC_DIR)/stm32f103xx_timer.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_dma.o:$(SRC_DIR)/stm32f103xx_dma.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_init.o:$(SRC_DIR)/stm32f103xx_init.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_serial.o:$(SRC_DIR)/stm32f103xx_serial.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_systick.o:$(SRC_DIR)/stm32f103xx_systick.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_utilities.o:$(UTIL_DIR)/stm32f103xx_utilities.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_fault_handlers.o:$(SRC_DIR)/stm32f103xx_fault_handlers.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
#seg_display.o: seg_display.c
#	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
$(MAIN).o:$(MAIN).c 
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
$(STARTUP).o:$(STARTUP).c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
$(SYSCALLS).o:$(SYSCALLS).c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
final.elf: $(MAIN).o $(DRIVERS) $(OTHERS) $(UTILS) $(DISPLAY) $(SENSORS) $(STARTUP).o $(SYSCALLS).o
	$(CC) $(LDFLAGS) $^ -o $@

final.bin:final.elf
	arm-none-eabi-objcopy -O binary $^ $@
load:final.bin
	$(FLASH) $^ $(FLASH_ADDR)

examine-sections:
	$(OBJ_DUMP) -h final.elf
examine-symbols:
	$(NM) final.elf
dump:final.elf
	$(OBJ_DUMP) -d $^ > logs/assembly_dump.txt
	$(OBJ_DUMP) -h $^ > logs/section_contents.txt
	$(OBJ_DUMP) -s $^ > logs/section_contents_detailed.txt
	$(NM) final.elf > logs/symbols.txt
probe:
	$(PROBE)

debug: openocd gdb
openocd:
	openocd -f $(OPENOCD_INTF_CFG_FILE) -f $(OPENOCD_TGT_CFG_FILE)
gdb:
	$(GDB) final.elf

.PHONY: clean clean-obj
clean:
	del /s *.o *.elf *.map *.log *.bin *.exe *.txt
clean-obj:
	del /s *.o