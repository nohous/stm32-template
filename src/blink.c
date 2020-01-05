#include <stdint.h>
#include <stdio.h>
#include "stm32f072xb.h"
 
#define REG32(addr) (*(volatile uint32_t *)(addr))
 
//#define GPIOC_BASE   0x40021000
// LED_{U,D,L,R}: PC{6..9}
#define GPIOC_MODER  REG32(GPIOC_BASE + 0x00)
#define GPIOC_OTYPER REG32(GPIOC_BASE + 0x04)
#define GPIOC_ODR    REG32(GPIOC_BASE + 0x14)
 
#define RCC_AHBENR REG32(RCC_BASE + 0x14)
 
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

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN; 
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/* Alternate modes, USART1 = AF1 */
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER9))  | (GPIO_MODER_MODER9_1); 
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER10)) | (GPIO_MODER_MODER9_1); 
	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(GPIO_AFRH_AFSEL9))  | (1 << GPIO_AFRH_AFSEL9_Pos);
	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(GPIO_AFRH_AFSEL10)) | (1 << GPIO_AFRH_AFSEL10_Pos);


	USART1->BRR = 0x1A1;
	USART1->CR1 = USART_CR1_TE | USART_CR1_UE; 

	const char *stringtosend = "Nazdar debile";
	const char *p = stringtosend;

	do {
		USART1->TDR = *p;
		while (!(USART1->ISR & USART_ISR_TXE));
	} while (*(++p));
	while (!(USART1->ISR & USART_ISR_TC));


    GPIOC_OTYPER = 0; //LEDU_PIN_MASK; // push-pull
    set_gpioc_moder(LEDU_PIN, 0x1); // general purpose output
    set_gpioc_moder(LEDL_PIN, 0x1); // general purpose output
    set_gpioc_moder(LEDD_PIN, 0x1); // general purpose output
    set_gpioc_moder(LEDR_PIN, 0x1); // general purpose output
	GPIOC_ODR = 0;

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
        //GPIOC_ODR |= 1 << LEDR_PIN; // output pin low -> LED ON
        //delay(1000000);
        //GPIOC_ODR &= ~(1 << LEDR_PIN); // output pin high-z -> LED OFF
    }
#endif
}
 
void SystemInit(void)
{
	/* (1) Test if PLL is used as System clock */
	/* (2) Select HSI as system clock */
	/* (3) Wait for HSI switched */
	/* (4) Disable the PLL */
	/* (5) Wait until PLLRDY is cleared */
	/* (6) Set the PLL multiplier to 12 */
	/* (7) Enable the PLL */
	/* (8) Wait until PLLRDY is set */
	/* (9) Select PLL as system clock */
	/* (10) Wait until the PLL is switched on */
	if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) /* (1) */
	{
		RCC->CFGR &= (uint32_t) (~RCC_CFGR_SW); /* (2) */
		while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI); /* (3) */
	}
	RCC->CR &= (uint32_t)(~RCC_CR_PLLON);/* (4) */
	while((RCC->CR & RCC_CR_PLLRDY) != 0); /* (5) */

	RCC->CFGR = RCC->CFGR & (~RCC_CFGR_PLLMUL) | (RCC_CFGR_PLLMUL12); /* (6) */
	RCC->CR |= RCC_CR_PLLON; /* (7) */
	while((RCC->CR & RCC_CR_PLLRDY) == 0); /* (8) */

	RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); /* (9) */
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); /* (10) */

    return;
}
 
