#TOOLCHAIN=/home/nohous/opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi/bin/arm-eabi-
TOOLCHAIN=/home/nohous/opt/gcc-arm-none-eabi-7-2018-q2-update/bin/arm-none-eabi-
CC=$(TOOLCHAIN)gcc
LD=$(TOOLCHAIN)gcc
AS=$(TOOLCHAIN)gcc
OBJCOPY=$(TOOLCHAIN)objcopy
CFLAGS=-mthumb -mcpu=cortex-m0 -Os -specs=nano.specs -specs=nosys.specs 
ASFLAGS=-mthumb -mcpu=cortex-m0 -x assembler 
LDFLAGS=-TSTM32F072RBTx_FLASH.ld -Wl,-Map=blink.map

all: blink
	
clean:
	-rm *.o
	-rm blink
	-rm blink.bin
	-rm blink.map

blink.o: blink.c
	$(CC) $(CFLAGS) -c $< -o $@
startup_stm32f072xb.o: startup_stm32f072xb.s
	$(AS) $(ASFLAGS) -c $< -o $@

blink: blink.o startup_stm32f072xb.o
	$(LD) $(CFLAGS) $(LDFLAGS) $^ -o $@
	$(OBJCOPY) $@ -S -O binary $@.bin

