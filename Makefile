TOOLCHAIN=/home/nohous/opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi/bin/arm-eabi-
CC=$(TOOLCHAIN)gcc
LD=$(TOOLCHAIN)gcc
AS=$(TOOLCHAIN)gcc
OBJCOPY=$(TOOLCHAIN)objcopy
CFLAGS=-mthumb -mcpu=cortex-m0 
ASFLAGS=-mthumb -mcpu=cortex-m0 -x assembler

LDFLAGS=-Tstm32f072b.ld -mthumb -mcpu=cortex-m0 -nostartfiles -s -Wl,--build-id=none 

all: blink
	
clean:
	-rm *.o
	-rm blink
	-rm blink.bin

blink.o: blink.c
	$(CC) $(CFLAGS) -c $< -o $@
startup_ARMCM0.o: startup_ARMCM0.S
	$(AS) $(AMFLAGS) -c $< -o $@

blink: blink.o startup_ARMCM0.o
	$(LD) $(LDFLAGS) $^ -o $@
	$(OBJCOPY) $@ -S --strip-unneeded -O binary $@.bin

