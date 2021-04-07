#include "stm32f10x.h"

void PORTB_5_INIT (void)
{
	RCC->APB2ENR |=RCC_APB2ENR_IOPBEN;
	
	GPIOB->CRL &= ~GPIO_CRL_MODE5_0;
	GPIOB->CRL |= GPIO_CRL_MODE5_1;
	GPIOB->CRL &= ~GPIO_CRL_CNF5;
}

int main (void)
{
	PORTB_5_INIT ();
	
	while(1)
	{
		GPIO->BSRR |= GPIO_BSRR_BS5;
		GPIO->BSRR |= GPIO_BSRR_BR5;
	}
	
}