#include "sys.h"//??:????  qq:624668529
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//????ucos,???????????.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ??	  
#endif


//int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
//{
//	USART_SendData(USART1,(u8)ch);	
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
//}
//////////////////////////////////////////////////////////////////////////////////	 
//?????????,??????,??????????
//ALIENTEK STM32F4??????
//??1???		   
//????@ALIENTEK
//????:www.openedv.com
//????:2014/6/10
//??:V1.5
//????,?????
//Copyright(C) ????????????? 2009-2019
//All rights reserved
//********************************************************************************
//V1.3???? 
//?????????????????.
//????printf???
//???????????.
//???printf????????bug
//V1.4????
//1,???????IO?bug
//2,???USART_RX_STA,????????????2?14??
//3,???USART_REC_LEN,????????????????(???2?14??)
//4,???EN_USART1_RX?????
//V1.5????
//1,????UCOSII???
////////////////////////////////////////////////////////////////////////////////// 	  
 
//////////////////////////////////////////////////////////////////
//??????,??printf??,??????use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//??????????                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//??_sys_exit()??????????    
_sys_exit(int x) 
{ 
	x = x; 
} 
//???fputc?? 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//????,??????   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //???????
//??1??????
//??,??USARTx->SR??????????   	
u8 USART_RX_BUF[USART_REC_LEN];     //????,??USART_REC_LEN???.
//????
//bit15,	??????
//bit14,	???0x0d
//bit13~0,	??????????
u16 USART_RX_STA=0;       //??????	
//???IO ??1 
//bound:???
void uart_init(u32 bound){
   //GPIO????
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

   //USART1 ?????
	USART_InitStructure.USART_BaudRate = bound;//?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????
  USART_Init(USART1, &USART_InitStructure); //?????1
	
  USART_Cmd(USART1, ENABLE);  //????1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//??????
	//Usart1 NVIC ??
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//??1????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC????
#endif
	
}
//void USART1_IRQHandler(void)                	//??1??????
//{
//	u8 Res;
//#if SYSTEM_SUPPORT_OS 		//??SYSTEM_SUPPORT_OS??,?????OS.
//	OSIntEnter();    
//#endif
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
//	{
//		Res =USART_ReceiveData(USART1);//(USART1->DR);	//????????
//		
//		if((USART_RX_STA&0x8000)==0)//?????
//		{
//			if(USART_RX_STA&0x4000)//????0x0d
//			{
//				if(Res!=0x0a)USART_RX_STA=0;//????,????
//				else USART_RX_STA|=0x8000;	//????? 
//			}
//			else //????0X0D
//			{	
//				if(Res==0x0d)USART_RX_STA|=0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//					USART_RX_STA++;
//					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//??????,??????	  
//				}		 
//			}
//		}   		 
//  } 
//#if SYSTEM_SUPPORT_OS 	//??SYSTEM_SUPPORT_OS??,?????OS.
//	OSIntExit();  											 
//#endif
//} 
#endif	

