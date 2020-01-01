#include <stdint.h>
 
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
#define LEDU_PIN_MASK (1UL<<LEDU_PIN)

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

    RCC_AHBENR |= RCC_AHBENR_GPIOCEN; // enable GPIOC clock
    GPIOC_OTYPER = 0; //LEDU_PIN_MASK; // push-pull
    set_gpioc_moder(LEDU_PIN, 0x1); // general purpose output
	GPIOC_ODR = LEDU_PIN_MASK;

#if 1
    while(1)
    {
        GPIOC_ODR |= LEDU_PIN_MASK; // output pin low -> LED ON
        delay(100000);
        GPIOC_ODR &= ~LEDU_PIN_MASK; // output pin high-z -> LED OFF
        delay(100000);
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

void __aeabi_unwind_cpp_pr0(void)
{
}
