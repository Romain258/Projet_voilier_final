#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h" 
#include "service_girouette.h"

void init_timerGi (int PSC, int ARR){ //ARR = 1440 PSC =0
	//activation horloge
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	
	LL_TIM_InitTypeDef TimerStructInit ;
	TimerStructInit.Autoreload = ARR ;
	TimerStructInit.Prescaler = PSC ; 
	TimerStructInit.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TimerStructInit.CounterMode = LL_TIM_COUNTERMODE_UP ;
	TimerStructInit.RepetitionCounter = 0 ;
	
	//envoi
	LL_TIM_Init(TIM3, &TimerStructInit) ;
	LL_TIM_EnableCounter(TIM3);
}

void init_GPIO_Girouette (void){
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_6 ,LL_GPIO_MODE_FLOATING );
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_7 ,LL_GPIO_MODE_FLOATING );
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_5 ,LL_GPIO_MODE_FLOATING );
}

void init_capture (void) {

	TIM3->CCMR1 |= 1;
	TIM3->CCMR1 &= ~(0<<1);
	
	TIM3->CCMR1 |= 1<<8;
	TIM3->CCMR1 &= ~(0<<9);
	
	
	TIM3->CCER &= ~(0<<1);
	TIM3->CCER &=~(0<<4);//CC1NP à 0
	TIM3->CCMR1 &= ~(0000<<4);
	
	
	TIM3->CCER &= ~(0<<5); // CC2P
	//CC2NP à 0
	TIM3->CCER &=~(0<<8);
	TIM3->CCMR1 &= ~(0000<<12);
	
	TIM3->SMCR |= 11;
	TIM3->SMCR &=~(0<<2);
	TIM3->CR1 |= 1;
	
}


int lecture_girouette (void) {
	int alpha = TIM3->CNT;
	alpha = alpha/4 ;
	return alpha;
}
int scrut_index (void){
	
	int index = LL_GPIO_ReadInputPort(GPIOA) & (1<<5);
	
	
	return index;
}
