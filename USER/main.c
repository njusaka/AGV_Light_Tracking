#include "__Header__/car.h"

int   encoder_left,encoder_right;
float ang=0,dst=0;
float ang_break_ref = 0, dst_break_ref = 0;
int   Flag_Left=0,Flag_Right=0;
int   servo_i=0;
int light_off_count = 0;	   //延时计数
extern int heartbeat;           // 心跳，在stm32f10x_it.c内定义
extern __mission_status mission_stat;

int main(void)
 {		
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init();	    	 //延时函数初始化
	uart_init(9600);	     //串口初始化为115200
//	JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口  *
//	JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试 *
	MY_NVIC_PriorityGroupConfig(2);	//=====设置中断分组
	LED_Init();			     //LED端口初始化

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
     
    // 模块初始化
    // 里程计
    init_Odom_Variables();
    AGV_PIDCtrlInit();

	TIM5_Int_Init(7199,49); //定时器中断5ms   49//

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
 
extern __pid yaw_pid;                           // 偏航自旋PID，这个PID是直接控制车辆旋转的，所以参数应该比较大
extern __pid yaw_assist_pid;                    // 偏航辅助锁定PID，这个PID是作用于速度环的外环的，所以参数很小

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
 
