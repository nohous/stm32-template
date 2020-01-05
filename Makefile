#TOOLCHAIN=/home/nohous/opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi/bin/arm-eabi-
TOOLCHAIN=/home/nohous/opt/gcc-arm-none-eabi-7-2018-q2-update/bin/arm-none-eabi-
CC=$(TOOLCHAIN)gcc
LD=$(TOOLCHAIN)gcc
AS=$(TOOLCHAIN)gcc
OBJCOPY=$(TOOLCHAIN)objcopy
CFLAGS=-mthumb -mcpu=cortex-m0 -Os -specs=nano.specs -specs=nosys.specs 
ASFLAGS=-mthumb -mcpu=cortex-m0 -x assembler 
LDFLAGS=-Tstm32f072b.ld  -nostartfiles -Wl,--build-id=none,-Map=blink.map

all: blink
	
clean:
	-rm *.o
	-rm blink
	-rm blink.bin
	-rm blink.map

blink.o: blink.c
	$(CC) $(CFLAGS) -c $< -o $@
startup_ARMCM0.o: startup_ARMCM0.S
	$(AS) $(ASFLAGS) -c $< -o $@
	$(OBJCOPY) --remove-section '.ARM.*' $@
	$(OBJCOPY) -N '__aeabi_unwind_cpp_pr0' $@

blink: blink.o startup_ARMCM0.o
	$(LD) $(CFLAGS) $(LDFLAGS) $^ -o $@
	$(OBJCOPY) $@ -S -O binary $@.bin

