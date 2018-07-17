#include "include/find.h"
#include "__Header__/car.h"



void AGV_360_YawCtrl(float yaw_tgt, float yaw) {  //360
    
	float Integral_Temp = yaw_pid.Integral;

	yaw_pid.PreErr = yaw_pid.Err;

    // 计算期望    
	yaw_pid.exp  = yaw_tgt;
	yaw_pid.Err  = yaw_pid.exp - yaw;
	
	
    
    // 设置一个小的死区
    if (fabs(yaw_pid.Err) <= 5.0f)
        yaw_pid.Err = 0.0f;
	
		if(yaw_pid.Err==0)
			AGV_360_YawCtrl(180, yaw_filtered);
			
			
        
    // P
	yaw_pid.Pout = yaw_pid.Kp * yaw_pid.Err;
	
    // I
	Integral_Temp += yaw_pid.Err * yaw_pid.dt;
	//pid->Integral = limit_Data(Integral_Temp, pid->IntegralUpper, pid->IntegralLower);
	yaw_pid.Iout = yaw_pid.Ki * Integral_Temp;
	yaw_pid.Iout = limit_Data(yaw_pid.Iout, yaw_pid.IntegralUpper, yaw_pid.IntegralLower);
    
    // D
	yaw_pid.Derivative = (yaw_pid.Err - yaw_pid.PreErr) / yaw_pid.dt;
	yaw_pid.Dout = yaw_pid.Kd * yaw_pid.Derivative;
    
	yaw_pid.output = yaw_pid.Pout + yaw_pid.Iout + yaw_pid.Dout;

                                              }

void AGV_SetMotorOut(short moto_L, short moto_R) {
    

    if(moto_L > 0)  {
        GPIO_ResetBits(GPIOB, GPIO_Pin_4);
        GPIO_SetBits(GPIOA, GPIO_Pin_12);
        PWML     = moto_L + Dead_zone;
										}
    else if(moto_L < 0) {
        GPIO_SetBits(GPIOB, GPIO_Pin_4);
        GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        PWML     = -moto_L + Dead_zone;
												}
    else
        PWML = 0;
    
		
    if(moto_R > 0) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_15);
        GPIO_SetBits(GPIOB, GPIO_Pin_5);
        PWMR     = moto_R + Dead_zone;
                   }
    else if(moto_R < 0) {
        GPIO_SetBits(GPIOA, GPIO_Pin_15);
        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        PWMR     = -moto_R + Dead_zone;
    }
    else
        PWMR = 0;
	
    //if((int)PWML<-7000) PWML=-7000;	//限幅
    if(PWML>PWMLMAX)  PWML=PWMLMAX;	///7000  3000 7.14		
    //if((int)PWMR<-7000) PWMR=-7000;	//限幅
    if(PWMR>PWMRMAX)  PWMR=PWMRMAX;	
}

