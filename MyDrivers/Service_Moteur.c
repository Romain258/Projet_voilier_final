#include "Service_Moteur.h"


void Init_Moteur (TIM_TypeDef * Timer,GPIO_TypeDef *GPIOx){
	
	//Allumage clock
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	//Initialisation Timer
	LL_TIM_InitTypeDef TimerIn;
	TimerIn.Prescaler=20;
	TimerIn.Autoreload=171;
	TimerIn.CounterMode=LL_TIM_COUNTERMODE_UP;
	TimerIn.RepetitionCounter=0;
	TimerIn.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(Timer,&TimerIn);
	LL_TIM_EnableCounter(Timer);
	
	//Initialisation GPIO(PWM)
	
	LL_GPIO_InitTypeDef GPIOTest;
	GPIOTest.Pin=LL_GPIO_PIN_1;
	GPIOTest.Mode=LL_GPIO_MODE_ALTERNATE;
	GPIOTest.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	GPIOTest.Speed=LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIOTest.Pull=0x0000000000000;
	LL_GPIO_Init(GPIOx,&GPIOTest);
	
	LL_TIM_OC_SetMode(TIM2,LL_TIM_CHANNEL_CH2,LL_TIM_OCMODE_PWM1);
	LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH2);
	
	//Initialisation GPIO (Sens)
	GPIOTest.Pin=LL_GPIO_PIN_2;
	GPIOTest.Mode=LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(GPIOx,&GPIOTest);
}


void Set_Param (int angle){
	//envoi du bon sens dans le plateau
	float dutycycle;
	int Compte;
	if (angle<0){
		LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_2);
	  dutycycle=-(4.0/3.0)*(float)angle;
	}
	else if (angle>=0){
		LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_2);
	  dutycycle=(4.0/3.0)*(float)angle;
	}
	//envoi de la vitesse (timer PWM)
		Compte=(dutycycle*171)/100;

	LL_TIM_OC_SetCompareCH2(TIM2,Compte);
}










