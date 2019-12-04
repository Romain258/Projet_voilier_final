/*recuperer l'angle de roulis :
accelerometre
*/
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h"
#include "service_voile.h"
#include "stm32f1xx_ll_usart.h"

void Init_ADC_roulis(void);

void corriger_roulis (void);


void init_usart (void) ; 

int get_roulis (int angle);

int get_angle_roulis (void);



