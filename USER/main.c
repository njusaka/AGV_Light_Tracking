#include "__Header__/car.h"

int   encoder_left,encoder_right;
float ang=0,dst=0;
float ang_break_ref = 0, dst_break_ref = 0;
int   Flag_Left=0,Flag_Right=0;
int   servo_i=0;
int light_off_count = 0;	   //��ʱ����
extern int heartbeat;           // ��������stm32f10x_it.c�ڶ���
extern __mission_status mission_stat;

int main(void)
 {		
	Stm32_Clock_Init(9);            //ϵͳʱ������
	delay_init();	    	 //��ʱ������ʼ��
	uart_init(9600);	     //���ڳ�ʼ��Ϊ115200
//	JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�  *
//	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ��� *
	MY_NVIC_PriorityGroupConfig(2);	//=====�����жϷ���
	LED_Init();			     //LED�˿ڳ�ʼ��

	KEY_Init();
	Motor_Init();
	PWM_Init(7199, 0);
	TIM3_PWM_Init(20000,72);
		
	OLED_Init();  
	MPU_Init();	
		
	Encoder_Init_TIM2();          
	Encoder_Init_TIM4();           
	

    OLED_P8x16Str(0,0,(unsigned char *)"Init MPU DMP..."); 
	while(mpu_dmp_init()) { }
    OLED_CLS();
     
    // ģ���ʼ��
    // ��̼�
    init_Odom_Variables();
    AGV_PIDCtrlInit();

	TIM5_Int_Init(7199,49); //��ʱ���ж�5ms   49//

    TIM_Cmd(TIM5, ENABLE);

    while(true) {	
			
		if (heartbeat % 10 == 0) {
            // 50ms
            display_Data();

			//USART_SendData(USART1, USART_RX_BUF[10]);//
			//refine_DataFrame((char *)USART_RX_BUF, angle_out, distance_out);
			//USART_SendData(USART1,ang);
			//USART_SendData(USART1,65);
        } 
		
 } 
 
}/*_Main_*/
 
extern __pid yaw_pid;                           // ƫ������PID�����PID��ֱ�ӿ��Ƴ�����ת�ģ����Բ���Ӧ�ñȽϴ�
extern __pid yaw_assist_pid;                    // ƫ����������PID�����PID���������ٶȻ����⻷�ģ����Բ�����С

void display_Data(void) {
    
    char str[40] = { 0 };
	
	memset(str, 0, sizeof(char) * 30);
    sprintf(str, "x: %3.3f y: %3.3f", pos_current.x, pos_current.y);
	OLED_P6x8Str(0,0, (unsigned char *)str);
	
//	OLED_P6x8Str(1,1,(unsigned char *)"x");
//	OLED_P6x8Num_8bit(10,1,pos_current.x);
//	OLED_P6x8Str(30,1,(unsigned char *)"y");
//	OLED_P6x8Num_8bit(40,1,pos_current.y);

//    memset(str, 0, sizeof(char) * 30);    
//	sprintf(str, "Roll: %3.3f", roll);
//  	OLED_P6x8Str(0,2, (unsigned char *)str);

    //memset(str, 0, sizeof(char) * 30);    
	//sprintf(str, "Yaw: %3.3f", yaw_current);             
	//OLED_P6x8Str(0,3, (unsigned char *)str);
			
	if(encoder_left<0) {
		OLED_P6x8Str(1,4,(unsigned char *)"encoderL  -");
		OLED_P6x8Num_8bit(70,4,-encoder_left);
	}
	else {
		
		OLED_P6x8Str(1,4,(unsigned char *)"encoderL  +");
		OLED_P6x8Num_8bit(70,4,encoder_left);
	}
	if(encoder_right<0) {
		OLED_P6x8Str(1,5,(unsigned char *)"encoderR  -");
		OLED_P6x8Num_8bit(70,5,-encoder_right);
	}
	else {
        OLED_P6x8Str(1,5,(unsigned char *)"encoderR  +");
        OLED_P6x8Num_8bit(70,5,encoder_right);
	}
    
	memset(str, 0, sizeof(char) * 30);
	sprintf(str, "ang: %3.1f", ang); 
	OLED_P6x8Str(60,1, (unsigned char *)str);
    
	memset(str, 0, sizeof(char) * 30);
    sprintf(str, "distance: %3.4f", dst);    
	OLED_P6x8Str(0,6, (unsigned char *)str);
	
    memset(str, 0, sizeof(char) * 30);    
    sprintf(str, "vel l: %d %d", TIM1->CCR1, TIM1->CCR4);
    OLED_P6x8Str(0,2, (unsigned char *)str);
   
    memset(str, 0, sizeof(char) * 30);
    sprintf(str, "yaw %3.3f %3.3f", yaw_assist_pid.output, yaw_assist_pid.output);
	OLED_P6x8Str(0,3, (unsigned char *)str);
    
}
 
