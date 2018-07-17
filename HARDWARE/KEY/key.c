#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "stm32f10x_exti.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//������ʼ������
int key_flag=0;
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line    = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


void EXTI2_IRQHandler(void)
{
//	delay_ms(10);
//	if(KEY2==0)
//	{
//		key_flag++;
//		if(key_flag==1)
//		{
//			Flag_Left=0;
//			Flag_Right=1;
//		}
//		if(key_flag==2)
//		{
//			Flag_Left=1;
//			Flag_Right=0;
//		}
//		if(key_flag==3)
//		{
//			Flag_Left=0;
//			Flag_Right=0;
//			flag_distance=10;
//		}
//	
//	}
//	EXTI_ClearITPendingBit(EXTI_Line2);
}
