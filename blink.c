#include <stdint.h>
#include <stdio.h>
 
#define REG32(addr) (*(volatile uint32_t *)(addr))
 
//#define GPIOC_BASE   0x40021000
// LED_{U,D,L,R}: PC{6..9}
#define GPIOC_BASE   0x48000800
#define GPIOC_MODER  REG32(GPIOC_BASE + 0x00)
#define GPIOC_OTYPER REG32(GPIOC_BASE + 0x04)
#define GPIOC_ODR    REG32(GPIOC_BASE + 0x14)
 
#define RCC_BASE 0x40021000
#define RCC_AHBENR REG32(RCC_BASE + 0x14)
#define RCC_AHBENR_GPIOCEN (1 << 19)
 
#define LEDU_PIN 6
#define LEDD_PIN 7
#define LEDL_PIN 8
#define LEDR_PIN 9

static
void delay(int nops)
{
    while(nops > 0)
    {
        asm ("nop");
        nops--;
    }
}
 
static
void set_gpioc_moder(int pin, int mode)
{
    uint32_t moder;
    uint32_t moder_pin_pos;
    uint32_t moder_pin_mask;
 
    moder_pin_pos = pin*2; // 2 bits per pin
    moder_pin_mask = 0x3UL << moder_pin_pos;
 
    moder = GPIOC_MODER; // read from register
    moder &= ~moder_pin_mask; // clear moder pin field
    moder |= (mode << moder_pin_pos); // set moder pin field
    GPIOC_MODER = moder; // write to register
}

void main(void)
{
	volatile uint32_t tmp;
	char ahoj[128];

    RCC_AHBENR |= RCC_AHBENR_GPIOCEN; // enable GPIOC clock
    GPIOC_OTYPER = 0; //LEDU_PIN_MASK; // push-pull
    set_gpioc_moder(LEDU_PIN, 0x1); // general purpose output
    set_gpioc_moder(LEDL_PIN, 0x1); // general purpose output
    set_gpioc_moder(LEDD_PIN, 0x1); // general purpose output
    set_gpioc_moder(LEDR_PIN, 0x1); // general purpose output
	GPIOC_ODR = 0;

	sprintf(ahoj, "%d", GPIOC_ODR);

#if 1
    while(1)
    {
        GPIOC_ODR |= 1 << LEDU_PIN; // output pin low -> LED ON
        delay(1000000);
        GPIOC_ODR &= ~(1 << LEDU_PIN); // output pin high-z -> LED OFF
        GPIOC_ODR |= 1 << LEDL_PIN; // output pin low -> LED ON
        delay(1000000);
        GPIOC_ODR &= ~(1 << LEDL_PIN); // output pin high-z -> LED OFF
        GPIOC_ODR |= 1 << LEDD_PIN; // output pin low -> LED ON
        delay(1000000);
        GPIOC_ODR &= ~(1 << LEDD_PIN); // output pin high-z -> LED OFF
        GPIOC_ODR |= 1 << LEDR_PIN; // output pin low -> LED ON
        delay(1000000);
        GPIOC_ODR &= ~(1 << LEDR_PIN); // output pin high-z -> LED OFF
    }
#endif
}
 
void SystemInit(void)
{
    return;
}
 
#if 0
void _exit(int code)
{
    while(1);
}
#endif 

#if 1
void _start(void)
{
	main();
}
#endif

#if 0
void __aeabi_unwind_cpp_pr0(void)
{
}
#endif
