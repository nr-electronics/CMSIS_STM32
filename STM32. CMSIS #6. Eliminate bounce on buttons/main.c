#include "stm32f10x.h"
#define SYSCLOCK 72000000U

#define LED_PC13_ON() GPIOC->BSRR |= GPIO_BSRR_BR13;
#define LED_PC13_OFF() GPIOC->BSRR |= GPIO_BSRR_BS13;


uint8_t Button_State = 0;
uint32_t Button_count = 0;
uint8_t Result = 0;


uint8_t Debounce_Handler (uint8_t Button_State)
{
	if(Button_State == 0)
	{
		if (Button_count < 10000)
		{
			Button_count++;
			return 0;
		} 
		else
		{
			if (Button_State == 0)
			{
				 return 1; //Кнопка гарантировано нажата
			}
		}
	}	
	else
	{
		if (Button_count > 0)
		{
			Button_count--;
			return 1;
		}
		else
		{
			if(Button_State == 1)
			{
				return 0; //Кнопка гарантирована не нажата
			}
		}
	}
}


void PINB_3_INIT (void)
{
	RCC->APB2ENR |=RCC_APB2ENR_IOPBEN;
	
	GPIOB->CRL &= ~GPIO_CRL_MODE3_0;
	GPIOB->CRL &= ~GPIO_CRL_MODE3_1;   
  
  GPIOB->CRL &= ~GPIO_CRL_CNF3_0;                          
  GPIOB->CRL |=  GPIO_CRL_CNF3_1;
  
  GPIOB->BSRR |= GPIO_BSRR_BS3;
}



void PINC_13_INIT (void) //LED PC13
{
	RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH |=  GPIO_CRH_MODE13_0;
	GPIOC->CRH |=  GPIO_CRH_MODE13_1;
                          
  GPIOC->CRH &= ~GPIO_CRH_CNF13_0;                          
  GPIOC->CRH &= ~GPIO_CRH_CNF13_1;
}


int main (void)
{
	PINC_13_INIT();
  PINB_3_INIT();
  
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); // set 1
  CLEAR_BIT(AFIO->MAPR,AFIO_MAPR_SWJ_CFG);   // set 0
  SET_BIT(AFIO->MAPR, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);
  
  
	while(1)
	{
    Button_State = READ_BIT(GPIOB->IDR, GPIO_IDR_IDR3); 
    Result = Debounce_Handler (Button_State);
    
    
    if(Result == 1)
      {
        LED_PC13_ON();
      } 
      else 
      {
        LED_PC13_OFF();     
      }

	}
}


