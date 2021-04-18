#include "stm32f10x.h"
#define SYSCLOCK 72000000U

#define TIM_EnableIT_UPDATE(TIMx) TIMx->DIER |= TIM_DIER_UIE//Макрос разрешения прерывания
#define TIM_EnableCounter(TIMx)   TIMx->CR1 |= TIM_CR1_CEN//Макрос включения таймера

#define PIN_B5_ON() GPIOB->BSRR |= GPIO_BSRR_BS5;
#define PIN_B5_OFF() GPIOB->BSRR |= GPIO_BSRR_BR5;

static void TIM3_Init (void)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    NVIC_EnableIRQ (TIM3_IRQn);
    WRITE_REG(TIM3->PSC, 7199);//72 000 000 /(7199 +1) = 10 000  за 1 сек
    WRITE_REG(TIM3->ARR, 10000); // 10 000 * 0,5c = 5000 - значение счета для 0,5с.
  }

__IO uint32_t SysTick_CNT = 0;
__IO uint8_t   tim3_cnt=0;

void SysTick_Init(void)
{
  SysTick->LOAD &= ~SysTick_LOAD_RELOAD_Msk;
  SysTick->LOAD = SYSCLOCK/1000 - 1;
  SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}


void SysTick_Handler(void)
{
  if(SysTick_CNT > 0)  
  {  
  SysTick_CNT--;
  }
}

void delay_ms(uint32_t ms)
{
  SysTick->VAL &= ~SysTick_VAL_CURRENT_Msk;
  SysTick->VAL = SYSCLOCK / 1000 - 1; 
  SysTick_CNT = ms;
  while(SysTick_CNT) {}
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
	
  TIM3_Init();
  TIM_EnableIT_UPDATE(TIM3);
  TIM_EnableCounter(TIM3);
  
	while(1)
	{
//		GPIOB->BSRR |= GPIO_BSRR_BS5;
//    delay_ms(1000);
//		GPIOB->BSRR |= GPIO_BSRR_BR5;
//    delay_ms(1000);
	}
}

void TIM3_IRQHandler (void)
{
  if(READ_BIT(TIM3->SR, TIM_SR_UIF))
  {
    CLEAR_BIT(TIM3->SR, TIM_SR_UIF);
    
    switch (tim3_cnt)
    {
      case 0: PIN_B5_ON(); break;
      case 1: PIN_B5_OFF(); break;
    }
    
    tim3_cnt++;
    
    if(tim3_cnt>1) 
    {  
      tim3_cnt = 0;
    }  
  }
  
  
}
