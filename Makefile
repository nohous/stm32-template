TOOLCHAIN=/home/nohous/opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi/bin/arm-eabi-
#SYSROOT=/home/nohous/opt/sysroot-newlib-linaro-2019.12-arm-eabi
CC=$(TOOLCHAIN)gcc
LD=$(TOOLCHAIN)gcc
AS=$(TOOLCHAIN)gcc
OBJCOPY=$(TOOLCHAIN)objcopy
COPTS=-mthumb -mcpu=cortex-m0 
ASOPTS=-mthumb -mcpu=cortex-m0 -x assembler

LDOPTS=-Tstm32f072b.ld -mthumb -mcpu=cortex-m0 -nostartfiles -s -Wl,--build-id=none 
#	   --sysroot=$(SYSROOT) \
#LDOPTS=-Tstm32f072b.ld -mthumb -mcpu=cortex-m0 -nostdlib -Wl,--build-id=none 
#LDOPTS=-Tgcc_arm.ld -mthumb -mcpu=cortex-m0

all: blink
	
clean:
	-rm *.o
	-rm blink
	-rm blink.bin

blink.o: blink.c
	$(CC) $(COPTS) -c $< -o $@
startup_ARMCM0.o: startup_ARMCM0.S
	$(AS) $(AMOPTS) -c $< -o $@

blink: blink.o startup_ARMCM0.o
	$(LD) $(LDOPTS) $^ -o $@
	$(OBJCOPY) $@ -S --strip-unneeded -O binary $@.bin

