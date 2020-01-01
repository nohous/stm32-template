Template for STM32 development
==============================
This project targeted on the STM32F072B-DISCO dev kit. It blinks the UP led.

Compilation prerequisities:

* Linaro 7 gcc-arm-eabi toolchain (or compatible), available at
  <https://releases.linaro.org/components/toolchain/binaries/latest-7/arm-eabi/>
* GNU make


Notes
-----
Startup code (startup_ARMCM0.S) is taken without change from ARM CMSIS v5.
Linker script is based on template also available within the CMSIS, originally under the name
gcc_arm.ld. To accomodate for different MCU, update `__ROM_BASE, __ROM_SIZE, __RAM_BASE,
__RAM_SIZE, __STACK_SIZE, __HEAP_SIZE` inside the script.

The `--nostartfiles` linker option is required to prevent automatic linkage of startup code
distributed with the linaro toolchain.

Important: the `-Wl,--build-id=none` is required to correctly link the program, otherwise
the linker automagically links in some GNU build-id section in front of all other sections, leading
in offset of reset / interrupt vector table and non-functional binary. This can probably be solved
by updating the linker script to move the section to some non-special location when there is the
need to have the included information during run-time.

Three functions need to be defined with this kind of setup:

* `void SystemInit(void)`, which is called from within the supplied assembly startup code during early init
  phase (you can configure clocks etc. from in there)
* `void _start(void)`, which is called by the supplied assembly startup code at the very end of the reset
  handler (we simply call `main()` from here)
* `void __aeabi_unwind_cpp_pr0(void)`, which is for reasons unknown referenced by the compiled startup
  code object file and later, during project linkage, causes linking in unneccessary c++ stack
  unwind infrastructure. Since this symbol has a weak linkage and is not actually used, we replace it here
  with dummy stub to prevent bloating the target binary.
