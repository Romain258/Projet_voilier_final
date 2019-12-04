#include "ServiceBatterie.h"

void Init_Batterie(GPIO_TypeDef *GPIOx){
	
	//Allumage des clocks Attention on sait pas si bon ADC
	//LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	
	//Initialisation ADC
/*	RCC->CFGR |= (1<<14);
	RCC->CFGR |= (1<<15);
	ADC1->CR2 |= (1<<0);
	ADC1->CR2 &=~(1<<1);
	ADC1->CR2 |=(1<<17);
	ADC1->CR2 |=(1<<18);
	ADC1->CR2 |=(1<<19);
	ADC1->SQR1 &=~(1<<20);
	ADC1->SQR1 &=~(1<<21);
	ADC1->SQR1 &=~(1<<22);
	ADC1->SQR1 &=~(1<<23);
	ADC1->SQR3 |= 0xC;*/
	
	//Initialisation GPIO
	/*LL_GPIO_InitTypeDef GPIOIn;
	GPIOIn.Pin=LL_GPIO_PIN_2;
	GPIOIn.Mode=LL_GPIO_MODE_ANALOG;
	GPIOIn.Speed=LL_GPIO_SPEED_FREQ_MEDIUM;
	LL_GPIO_Init(GPIOx,&GPIOIn);*/
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_2 ,LL_GPIO_MODE_ANALOG );
	ADC1->CR2|=(1<<22);
  }

	void UART_conf(void){
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_USART_InitTypeDef uart;
	uart.BaudRate = 19200;
	uart.DataWidth = LL_USART_DATAWIDTH_8B ;
	uart.HardwareFlowControl = LL_USART_HWCONTROL_NONE ;
	uart.OverSampling = LL_USART_OVERSAMPLING_16 ;
	uart.Parity=LL_USART_PARITY_NONE;
	uart.StopBits=LL_USART_STOPBITS_2;
	uart.TransferDirection=LL_USART_DIRECTION_TX_RX ;
	LL_USART_Init(USART2,&uart);
	LL_USART_Enable(USART2);
	
}
	
void Detecter_Low_Batterie(){
	
	//char tabMessage[11];
	char tabMessage[11] = {'L', 'O', 'W', ' ','B', 'A','T','T',0x0D};
	ADC1->SQR3 = 0xC;
	//attention en ascii
	
	/*tabMessage[0] = 0x41;
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

static short Voltage = 0x360;
	ADC1->CR2|=(1<<22);	
ADC1->CR2 |= (1<<0);
	ADC1->CR2|=(1<<22);	
while (!(((ADC1->SR)&(1<<1))==0x2)){};
	Voltage=ADC1->DR;
		ADC1->SR&=~(1<<1);
	

	if (Voltage<0x350){
	
	//envoyer le message
		for (int i=0 ; i<11 ; i++){
		
			while (!LL_USART_IsActiveFlag_TXE(USART1))
			{
			}
		LL_USART_TransmitData8(USART1,tabMessage[i]);

		}
	}
	}

