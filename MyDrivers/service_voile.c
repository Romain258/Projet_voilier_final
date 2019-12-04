#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h" 
#include "service_voile.h"

#define ANGLE_VOILE_MAX 60.
#define VAL_MIN 0.
#define MIN_PWM 499.
#define MAX_PWM 900.
#define TETA_MAX 80.
#define TETA_MIN 20.

#define COEF_DIR (MAX_PWM-MIN_PWM)/(TETA_MAX-TETA_MIN)
#define ORD_OR 415.

void init_timer (int PSC, int ARR){
	//activation horloge
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	
	LL_TIM_InitTypeDef TimerStructInit ;
	TimerStructInit.Autoreload = ARR ;
	TimerStructInit.Prescaler = PSC ; 
	TimerStructInit.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TimerStructInit.CounterMode = LL_TIM_COUNTERMODE_UP ;
	TimerStructInit.RepetitionCounter = 0 ;
	
	//envoi
	LL_TIM_Init(TIM1, &TimerStructInit) ;
	LL_TIM_EnableCounter(TIM1);
}

void init_PWM (int compvalue) {
	LL_TIM_OC_InitTypeDef PWMStructInit ;
	PWMStructInit.OCMode=LL_TIM_OCMODE_PWM1  ;
	PWMStructInit.OCPolarity  = LL_TIM_OCPOLARITY_HIGH ;
	PWMStructInit.CompareValue = compvalue ; 
	PWMStructInit.OCState = LL_TIM_OCSTATE_ENABLE ;
	PWMStructInit.OCNIdleState =LL_TIM_OCIDLESTATE_LOW;
	PWMStructInit.OCNPolarity =LL_TIM_OCPOLARITY_HIGH;
	PWMStructInit.OCNState =LL_TIM_OCSTATE_DISABLE;
	PWMStructInit.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
	LL_TIM_OC_Init(TIM1,LL_TIM_CHANNEL_CH1,&PWMStructInit) ;
	TIM1->BDTR |= 1<<15;
}
void set_comp_value (int compvalue){
	TIM1->CCR1 = compvalue;
}

void init_GPIO_PWM (void){
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_8 ,LL_GPIO_MODE_ALTERNATE );
	LL_GPIO_SetPinOutputType(GPIOA,LL_GPIO_PIN_8 ,LL_GPIO_OUTPUT_PUSHPULL );
	
	
}

int calcul_tension_servo (int teta, int ARR){
	int comp_value ;
	
	if (teta == 0){
		comp_value = MIN_PWM;
	}
	else {
	comp_value = (int) (COEF_DIR*teta+ORD_OR);
	}
	return comp_value;
}

int calcul_teta (int alpha){
	int teta ;
	//alpha doit etre en degres
	if ((alpha>= 45 & alpha <=180)) {
		teta=(int)(alpha*(ANGLE_VOILE_MAX/(180.-45.))) ; }
	else if (alpha >180 & alpha <315){
		teta=(int)((360-alpha)*(ANGLE_VOILE_MAX/(315.-180.))) ; }
	else if ((alpha >=0 & alpha <45) | (alpha> 315 & alpha <=360)){
		teta=0 ; }
	
	return teta ;
}

