#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 
#include "stm32f1xx_ll_gpio.h"
#include "service_roulis.h"
#include "service_voile.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_adc.h"


void Init_ADC_roulis(){
	
	//Allumage des clocks Attention on sait pas si bon ADC
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

	RCC->CFGR |= (11<<14);
	
	
	//Initialisation ADC
	ADC1->CR2 |= (1<<0);
	ADC1->SQR3 |= 0xB;
	ADC1->CR2 &=~(0<<1);
	ADC1->CR2 |= (111<<17);
	ADC1->CR2 |= (1<<22);
	
		//Initialisation GPIO
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_1 ,LL_GPIO_MODE_ANALOG );
	//LL_GPIO_SetPinSpeed (GPIOC,LL_GPIO_PIN_1,LL_GPIO_SPEED_FREQ_MEDIUM );

	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_11 );
	//LL_GPIO_InitTypeDef GPIOIn;
	//GPIOIn.Pin=LL_GPIO_PIN_1;
	//GPIOIn.Mode=LL_GPIO_MODE_ANALOG;
	//GPIOIn.Speed=LL_GPIO_SPEED_FREQ_MEDIUM;
	//LL_GPIO_Init(GPIOC,&GPIOIn);
}
void corriger_roulis (){
	char tabMessage[11] = {'A', 'T', 'T', 'E','N', 'T','I','O','N',0x0D};
	
	//attention en ascii
	
	/*tabMessage[0] = 'A';
	tabMessage[1] = 0x74;
	tabMessage[2] = 0x74;
	tabMessage[3] = 0x65;
	tabMessage[4] = 0x6E;
	tabMessage[5] = 0x74;
	tabMessage[6] = 0x69;
	tabMessage[7] = 0x6F;
	tabMessage[8] = 0x6E;
	tabMessage[9] = 0x21;
	tabMessage[10] = 0x0D;*/
	//envoyer le message
		for (int i=0 ; i<11 ; i++){
		
			while (!LL_USART_IsActiveFlag_TXE(USART1))
			{
			}
		LL_USART_TransmitData8(USART1,tabMessage[i]);

		}

	//lacher les voiles
	int comp ;
	int teta = 0 ;
	int ARR = 9999 ;
	comp = calcul_tension_servo(teta,ARR);
	
	
	while (get_roulis(get_angle_roulis()) !=0){
	set_comp_value (comp);}
	
}

void init_usart() {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	LL_USART_InitTypeDef  MyUSARTInit;
	MyUSARTInit.BaudRate = 9600;
	MyUSARTInit.DataWidth = LL_USART_DATAWIDTH_8B;
	MyUSARTInit.StopBits = LL_USART_STOPBITS_1 ;
	MyUSARTInit.Parity = LL_USART_PARITY_NONE ;
	MyUSARTInit.TransferDirection =LL_USART_DIRECTION_TX ;
	MyUSARTInit.HardwareFlowControl = LL_USART_HWCONTROL_NONE  ;
	MyUSARTInit.OverSampling = LL_USART_OVERSAMPLING_16  ;	
	LL_USART_Init (USART1,&MyUSARTInit);
	
	
	//Configuration de l'USART
	LL_GPIO_InitTypeDef uart_tx;
	uart_tx.Pin=LL_GPIO_PIN_9;
	uart_tx.Mode=LL_GPIO_MODE_ALTERNATE;
	uart_tx.Pull=LL_GPIO_PULL_DOWN;
	uart_tx.Speed=LL_GPIO_SPEED_FREQ_LOW;
	uart_tx.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	
	//Enable le Tx associé à l'émetteur (PA11)
	LL_GPIO_InitTypeDef uart_en;
	uart_en.Pin=LL_GPIO_PIN_11;
	uart_en.Mode=LL_GPIO_MODE_OUTPUT;
	uart_en.Pull=LL_GPIO_PULL_UP;
	uart_en.Speed=LL_GPIO_SPEED_FREQ_LOW;
uart_en.OutputType=LL_GPIO_OUTPUT_PUSHPULL;
	LL_GPIO_Init(GPIOA, &uart_tx);
	LL_GPIO_Init(GPIOA, &uart_en);
	LL_USART_Enable(USART1);
 
	LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11);
	 //Alternate output GPIO
	 
	 //LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_11 ,LL_GPIO_MODE_ALTERNATE );
	//LL_GPIO_AF_EnableRemap_USART1();
	//LL_USART_EnableIT_TXE(USART1);
}

int get_roulis (int angle){
	int tangage =0 ;
	if ((angle < 0x085C)) 
	{	tangage =1 ;
	} 
	else {
		tangage = 0 ;
	}
	return tangage ;
}

int get_angle_roulis (void) {
	int angle_roulis;
	ADC1->SQR3 = 0xB;
	
	ADC1->CR2 |= (1<<22);
	ADC1->CR2 |= (1<<0);
  ADC1->CR2 |= (1<<22);
	while(! ((ADC1->SR&(1<<1))==0x2)){
		
		}
	

		ADC1->SR&=~(0<<1);
		angle_roulis=ADC1->DR;	
		
		
	
	
	return angle_roulis;
}

