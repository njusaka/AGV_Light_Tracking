#include "car.h"
#include "motor.h"
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_5;  //B4,B5
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12|GPIO_Pin_15;	//A12,A15
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void PWM_Init(int arr, int psc)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_11; //
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;       //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period        = arr;   	 
	TIM_TimeBaseStructure.TIM_Prescaler     = psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);	//MOE �����ʹ��
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
}
	
void Set_motor(int moto_L, int moto_R)
{
		if(roll>40||roll<-40)
		{
			PWML=0;
			PWMR=0;
		}
		if(moto_L > 0)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_15);
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
			PWML     = moto_L + Dead_zone;
		}
		else if(moto_L < 0)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_15);
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
			PWML     = -moto_L + Dead_zone;
		}
			      
		//if((int)PWML<-7000) PWML=-7000;	//�޷�
		if(PWML>1000)  PWML=1000;	 //
		
		if(moto_R > 0)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_4);
			GPIO_ResetBits(GPIOA, GPIO_Pin_12);
			PWMR     = moto_R + Dead_zone;
		}
		else if(moto_R < 0)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_4);
			GPIO_SetBits(GPIOA, GPIO_Pin_12);
			PWMR     = -moto_R + Dead_zone;
		}
		
        //if((int)PWMR<-7000) PWMR=-7000;	//�޷�
		if(PWMR>1000)  PWMR=1000;	

}


