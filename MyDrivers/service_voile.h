#include "stm32f103xb.h" 

void init_timer (int PSC, int ARR) ;
void init_PWM (int compvalue) ;
void set_comp_value (int compvalue);
void init_GPIO_PWM (void) ;
int calcul_tension_servo (int teta, int ARR);
int calcul_teta (int alpha) ;
