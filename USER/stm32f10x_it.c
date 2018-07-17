/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "__Header__/car.h"
#include "find.h"
#include "data.h"

//float ang, distance;
__mission_status mission_stat = moving;
bool light_off_count_start;

int heartbeat = 0;              // 心跳，以5ms为单位记录
float flag_break_ref = 0;
float speed_cut = 40;
void TIM5_IRQHandler(void)   
{

    // 5ms
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
    {	
		if(light_off_count_start == 1) light_off_count++;
		//data
        encoder_left  = (short)TIM2->CNT;                   TIM2->CNT = 0;
        encoder_right = (short)(0xFFFFFFFF-TIM4->CNT) + 1;  TIM4->CNT = 0;
                
        // 方向调整
        encoder_left  = -encoder_left;
        encoder_right = -encoder_right;
                
        update_Odom_IMU();
		
        
        if (((dst == 0 && ang == 0) || (dst > 20 && fabs(ang) < 30)) && light_off_count == 0) {		/// 20 < dst < 45
            mission_stat =  moving;
        }
        else if ((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0 || GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0 )|| dst < 15) {
            mission_stat = stopped;
        }
//        else {
//            mission_stat = moving;
//        }
        
        if (mission_stat == moving) {
            if (fabs(ang) < 30 && dst > speed_cut && dst < 500) {									// ang < 25
                AGV_Tracking_Angle_Dst_Ctrl(ang, dst);
                
                if (flag_break_ref == 0) {
                    dst_break_ref = dst;
                    ang_break_ref = ang;
                }
                if(dst_break_ref > 130) speed_cut = 70;				// 150, 55
				else if (dst_break_ref > 180) speed_cut = 100;		// 200, 90
                else if (dst_break_ref > 260) speed_cut = 150;		// 300, 120
                else if (dst_break_ref > 360) speed_cut = 180;		// 400, 140
				else 						  speed_cut = 50;
                flag_break_ref++;
            }         
           else if (fabs(ang) < 45 && dst > 20) {
                       
                AGV_Tracking_Dst_Precise_Control(ang, dst);
            }
			else if (fabs(ang) < 10 && dst > 1 && dst < 15 ) {
				mission_stat = stopped;
			}
            else if (fabs(ang) < 50 && !(dst == 0.0f && ang == 0.0f))
                AGV_Tracking_Simple_Spin(25);
            else{
                AGV_Tracking_Simple_Spin(40); 
				flag_break_ref = 0;
			}
		}
        else if (mission_stat == stopped) {
            AGV_VelCtrl(0, 0, vel_l, vel_r);
			//AGV_SetMotorOut(0, 0);
            mission_stat = light_off;
        }
        if (mission_stat == light_off) {
            //AGV_VelCtrl(0, 0, vel_l, vel_r);
            AGV_SetMotorOut(0, 0);
			light_off_count_start = 1;
            if(light_off_count == 20/5)		set_Servo_Angles(0);
			if(light_off_count == 200/5)	set_Servo_Angles(30);
			if(light_off_count == 360/5)	set_Servo_Angles(60);
			if(light_off_count == 520/5)	set_Servo_Angles(90);
            if(light_off_count == 680/5)	set_Servo_Angles(120);
            if(light_off_count == 1200/5)	set_Servo_Angles(0);      //800太快，延时
            if(light_off_count == 1600/5)	TIM_SetCompare1(TIM3, 0);
            if (light_off_count >= 1620/5 && light_off_count < 2350 / 5) {
                // 倒车
                //AGV_VelCtrl(-30, -30, vel_l, vel_r);
				AGV_SetMotorOut(-2000, -2000);
            }
            if (light_off_count >= 2350 / 5 && light_off_count <= 2800 / 5) {   //1500
                // 顺时针旋转45度以避开当前的灯
                AGV_Tracking_Simple_Spin(40);    //15
            }
             if (light_off_count >= 2800 / 5) {
                light_off_count_start = false;
                light_off_count = false;
				 
                mission_stat  = moving;
             }       
        }
//        else
//            mission_stat = moving;
        	
	
        //AGV_360_YawCtrl(180, yaw_filtered);//360
        //AGV_SetMotorOut(yaw_pid.output, -yaw_pid.output);//360
        
        // 测试1，转向测试
        //AGV_360_YawCtrl(180, yaw_filtered);
        //AGV_YawCtrl(360, yaw_filtered);
        // AGV_SetMotorOut(yaw_pid.output, -yaw_pid.output);
        
        // 测试2，直线测试
        //AGV_Pos_Vel_Ctrl_Straight(20, dst_agv, vel_l, vel_r);
        //AGV_VelCtrl(5.0f, 5.0f, vel_l, vel_r);
        
        // 测试3，直线+转向直接耦合控制测试
        //char USART_RX_BUF[256] = { "S10.2564 105.672ES-8.378 98.135ES3.15 67.123" };//        
        
//		set_Servo_Angles(0);
//		delay_ms(400);		
//		set_Servo_Angles(90);
//		delay_ms(400);	
//		TIM_SetCompare1(TIM3, 0);
//		delay_ms(400);        	 
//		TIM_SetCompare1(TIM3, 0);				
        
    
        heartbeat++;
    }
    
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志 		
		
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{	
	int space_pos = -1, s_pos = -1, e_pos = -1;
	int i = 0, j = 0;
	char temp[20] = { 0 };
	
	// https://blog.csdn.net/dearwind153/article/details/30601813
    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) 
    {  
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1, USART_FLAG_ORE);
    }  

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{

		USART_RX_BUF[USART_RX_STA++] = (u8)USART_ReceiveData(USART1);
		
		if (USART_RX_STA >= USART_REC_LEN - 1) {
			memset(USART_RX_BUF, 0, sizeof(u8) * USART_REC_LEN);
			USART_RX_STA = 0;
		}
		
		if (USART_RX_BUF[USART_RX_STA - 1] == 'E' && USART_RX_STA > 15) {
			
			e_pos = USART_RX_STA - 1;
			
			for (i = e_pos; i >= 0; i--) {
				if (USART_RX_BUF[i] == ' ') {
					space_pos = i;
					break;
				}
			}
			if (space_pos != i) {
				return;
			}
			for (i = space_pos + 1, j = 0; i < e_pos; i++, j++)
				temp[j] = USART_RX_BUF[i];
			ang = atof(temp);
			
			for (i = space_pos; i >= 0; i--) {
				if (USART_RX_BUF[i] == 'S') {
					s_pos = i;
					break;
				}
			}
			for (i = s_pos + 1, j = 0; i < space_pos; i++, j++)
				temp[j] = USART_RX_BUF[i];
			dst = atof(temp);	
			
		}
			
		
    } 
	
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);

} 
 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
//  while (1)
//  {
//  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
//  while (1)
//  {
//  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
//  while (1)
//  {
//  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
//  while (1)
//  {
//  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
