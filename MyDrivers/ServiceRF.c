#include "ServiceRF.h"


void Init_RF(TIM_TypeDef * Timer,GPIO_TypeDef *GPIOx){
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	//Initialisation Timer
	LL_TIM_InitTypeDef TimerIn;
	TimerIn.Prescaler=200;
	TimerIn.Autoreload=7199;
	TimerIn.CounterMode=LL_TIM_COUNTERMODE_UP;
	TimerIn.RepetitionCounter=0;
	TimerIn.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(Timer,&TimerIn);
	
	//Gestion de la PWM
	TIM4->CCMR1&=~(1<<1);
	TIM4->CCMR1|=(1<<0);
	TIM4->CCER &=~(1<<1);
	TIM4->CCMR1 |=(1<<9);
	TIM4->CCMR1&=~(1<<8);
	TIM4->CCER |=(1<<5);
	TIM4->SMCR |=(1<<6);
	TIM4->SMCR |=(1<<4);
	TIM4->SMCR &=~(1<<5);
	TIM4->SMCR |=(1<<2);
	TIM4->SMCR &=~(1<<1);
	TIM4->SMCR &=~(1<<0);
	TIM4->CCER |=(1<<4);
	TIM4->CCER |=(1<<0);
	
	//Initialisation GPIO(PWM)
	
	LL_GPIO_InitTypeDef GPIOIn;
	GPIOIn.Pin=LL_GPIO_PIN_6;
	GPIOIn.Mode=LL_GPIO_MODE_FLOATING;
	GPIOIn.Speed=LL_GPIO_SPEED_FREQ_MEDIUM;
	LL_GPIO_Init(GPIOx,&GPIOIn);
	
	LL_TIM_EnableCounter(Timer);

}



float get_offset(){
	float offset1;
	int compteur=0;
	//int i;
	//float x=0.0;
	//for (i=0;i<10;i++){
while (compteur < 100){
		if (TIM4->CNT == 7199){
			compteur++;
		}
	}
	
 compteur=0;

	offset1 =(float)(TIM4->CCR2)/(float)(TIM4->CCR1);
	//if (i>0)
	//x=(x+offset)/2;
	//else 
	//x=offset;
		while (compteur < 100){
		if (TIM4->CNT == 7199){
			compteur++;
		}
	}
	
		return offset1;
}
	
int get_angle(float offset){
	int angle;
	float DutyCycle;	
	//Calcul du Duty Cycle
	DutyCycle=(float)(TIM4->CCR2)/(float)(TIM4->CCR1);
	angle=(int)(((DutyCycle-offset)*2000));		
	return angle;
}








