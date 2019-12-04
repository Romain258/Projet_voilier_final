/**
  ******************************************************************************
  * @file    Templates_LL/Src/main.c
  * @author  MCD Application Team
  * @brief   Main program body through the LL API
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Config
#include "Service_Moteur.h"
#include "ServiceRF.h"
#include "stm32f1xx_ll_tim.h"
#include "ServiceBatterie.h"
#include "service_girouette.h"
#include "service_voile.h"
#include "service_roulis.h"
void  SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	

int main(void)
{

	  //float offset=0.0; 
		int alpha ; 
		int teta;
		int comp;
		int index =0;
		int flag_roulis =0 ;
		int ARR = 9999;
	  int angle_roulis =0x8FF ;
		static float offset=0.0;
		static int C=0;
	
	SystemClock_Config();
	Init_RF(TIM4,GPIOB);
	Init_Batterie(GPIOC);
	Init_Moteur(TIM2,GPIOA);
	init_timer(143,ARR) ;
	init_GPIO_PWM () ;
	init_PWM(499);
	init_timerGi(0,1440) ;
	init_GPIO_Girouette ();
	init_capture() ;
	Init_ADC_roulis();
	init_usart();
	

	offset=get_offset();
	
	
	
	
	
//ATTENDRE LE PREMIER TOUR DE GIROUETTE POUR COMMENCER
	while (index!=32){
		index = scrut_index();
	}
// fonction background		
			while (1){
				angle_roulis = get_angle_roulis();
				flag_roulis = get_roulis(angle_roulis);
				if (flag_roulis == 1) {
					
					corriger_roulis ();
				}
				
			//detection index girouette
			index = scrut_index();
				if (index == 32) {
					alpha = 0;
				}
				else{
					//lecture de la girouette
					alpha = lecture_girouette();
				}
				//commande voile
			teta = calcul_teta(alpha);
			comp = calcul_tension_servo(teta,ARR);
			set_comp_value (comp);
				
			
			C=get_angle(offset);
			Set_Param(C);
			Detecter_Low_Batterie();
			
			
				
			/*static float offset=0.0;
			static int C=0;
		  offset=get_offset();
			
			
		
			while (1){
				C=get_angle(offset);
				Set_Param(C);
				Detecter_Low_Batterie();
			}	
		*/
			
		}

	}








/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Set FLASH latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE oscillator */
	// ********* Commenter la ligne ci-dessous pour MCBSTM32 *****************
	// ********* Conserver la ligne si Nucléo*********************************
  //LL_RCC_HSE_EnableBypass();
  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };

  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 72MHz */
  LL_Init1msTick(72000000); // utile lorsqu'on utilise la fonction LL_mDelay

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);
}



/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
