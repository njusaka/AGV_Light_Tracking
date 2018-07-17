#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;       //����״̬���	 ָʾλ  
  
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

//void USART1_IRQHandler(void)                	//����1�жϷ������
//{
//		u8 Res  = 0;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntEnter();    	
//#endif
//	
//	// https://blog.csdn.net/dearwind153/article/details/30601813
//    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) 
//    {  
//        USART_ReceiveData(USART1);
//        USART_ClearFlag(USART1, USART_FLAG_ORE);
//    }  

//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res = (u8)USART_ReceiveData(USART1);	//��ȡ���յ������� 

//		USART_RX_BUF[USART_RX_STA] = Res;
//			
//		USART_RX_STA++; //����ֵ
//		
//		if (USART_RX_STA >= USART_REC_LEN) {
//			memset(USART_RX_BUF, 0, sizeof(u8) * USART_REC_LEN);
//			USART_RX_STA = 0;
//		}
//		
//    } 
//	
//	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	
//#if SYSTEM_SUPPORT_OS   //���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//	OSIntExit();  											 
//#endif
//} 

//float ang, v;

//int refine_DataFrame(char ker[]) 
//{
// 	
// 	char *ptrE,*ptrS,*ptrS2,*ptrB,*ptrB2,E='E',S='S',B=' ';
// 		// ptrE: E??
//		// ptrS: S??
//		// ptrS2: S????
//		// ptrB:  ??
//		// ptrB2: ??????? 
//		int i,j;

//   char str1[10],str2[10];
//		    
//	ptrE=strrchr(USART_RX_BUF,E);
//		
//	if (ptrE <= 0) {
//		printf("no found\n");
//		return -1;
//	}
//	printf("ptrE=%c\n", USART_RX_BUF[ptrE - USART_RX_BUF]);

//	printf("%s", ker);

//    for(ptrB2=ptrE; ptrB2 > USART_RX_BUF;ptrB2--)
//    {
//    if(*ptrB2==' ')
//    {
//    	printf("ptrB2=%d\n",ptrB2);
//    	break;
//	}
//    }
//	for(ptrS=ptrB2;  ptrS > USART_RX_BUF;ptrS--)
//    {
//    if(*ptrS==S)
//    {
//    	printf("ptrS=%d\n",ptrS);
//    	break;
//	}
//	
//	} 

//    ptrS=ptrS+1;
//    ptrB2=ptrB2+1;
//    
//	while(ptrS<ptrB2)
//	{
//	for(i=0;i<=10;i++)
//	{
//			str1[i] =*ptrS;
//			ptrS++;		
//		}	
//		v=atof(str1); 
//	printf("v=%.3f\n",v);
//		
//	}
//	
//	while(ptrB2<ptrE)
//	{
//	
//        for(j=0;j<=10;j++)
//	{
//			str2[j] =*ptrB2;
//			ptrB2++;		
//		}	
//		ang=atof(str2); 
//    printf("ang=%.3f\n",ang);
//    
//    
//	}
//	
//	
//	return 0;
// 	
// 	
// }

#endif	

///
