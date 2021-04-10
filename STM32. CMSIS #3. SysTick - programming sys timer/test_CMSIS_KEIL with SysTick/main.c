#include "stm32f10x.h"
#define SYSCLOCK 72000000U // макрос нашей системной частоты


__IO uint32_t SysTick_CNT = 0; //обьявляем и иницализируем в 0 значение нашего счетчика SysTick

void SysTick_Init(void)
{
  SysTick->LOAD &= ~SysTick_LOAD_RELOAD_Msk; //сбрасываем возможные старые значения интервала счета в 0
  SysTick->LOAD = SYSCLOCK/(1000 - 1); //чтобы считать как один цикл это 1 мс
  SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk; //сбрасываем текущие значения счетчика в 0
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;//запуск счетчика
  //выбрали частоту синхронизации - от тактирования процессора(АНВ),разрешили прерывания и включили счетчик.
}


void SysTick_Handler(void)
{
  if(SysTick_CNT > 0)  SysTick_CNT--;
}

void delay_mS(uint32_t mS)
{
  SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;//сбрасываем старые возможные значения текущего счета в 0
  SysTick->VAL = SYSCLOCK / (1000 - 1); // для точного отсчета в 1 мс
  SysTick_CNT = mS;
  while(SysTick_CNT) {}// как только будет 0, то выходим из цикла и задержка заканчивается
}

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
  SysTick_Init();
	
	while(1)
	{
		GPIOB->BSRR |= GPIO_BSRR_BS5;
    delay_mS(1000);
		GPIOB->BSRR |= GPIO_BSRR_BR5;
    delay_mS(1000);
	}
  
}
