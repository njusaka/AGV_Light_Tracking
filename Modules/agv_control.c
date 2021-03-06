#include "include/agv_control.h"

__pid yaw_pid;                           // 偏航自旋PID，这个PID是直接控制车辆旋转的，所以参数应该比较大
__pid yaw_assist_pid;                    // 偏航辅助锁定PID，这个PID是作用于速度环的外环的，所以参数很小
__pid pos_pid;
__pid vel_pid_l, vel_pid_r;              // 走直线时，速度环作为内环，角度环和位置环作为外环，自旋时仅使用角度环

void AGV_PIDCtrlInit(void) {
    
    __pid_cfg pos_pid_cfg;
    __pid_cfg yaw_pid_cfg;
    __pid_cfg vel_pid_cfg;
    __pid_cfg yaw_assist_pid_cfg;
    
    vel_pid_cfg.Kp = 3.0f;                       //5   				       //200 //100
    vel_pid_cfg.Ki = 0.5f;                                               //6.5
    vel_pid_cfg.Kd = 0.0000f;                                               //0.0
    init_PIDObject(&vel_pid_l, vel_pid_cfg, 500, -500, 0.05);             // 10ms
    init_PIDObject(&vel_pid_r, vel_pid_cfg, 500, -500, 0.05);
    
    pos_pid_cfg.Kp = 1.50f;
    pos_pid_cfg.Ki = 0.02f;         //0.36                                    // 0.36
    pos_pid_cfg.Kd = 0.00f;                                             // 0.05
    init_PIDObject(&pos_pid, pos_pid_cfg, 1, -1, 0.01);

    yaw_pid_cfg.Kp = 25.40f;   //10.4    //100.04       //0.08                                 // 0.80f 30.4
    yaw_pid_cfg.Ki = 0.01f;                                             // 0.02f
    yaw_pid_cfg.Kd = 0.05f;                                             // 0.12f
    init_PIDObject(&yaw_pid, yaw_pid_cfg, 1, -1, 0.005);
	
 /* yaw_pid_cfg.Kp = 100.40f;                                           // 0.80f
    yaw_pid_cfg.Ki = 0.01f;                                             // 0.02f
    yaw_pid_cfg.Kd = 0.05f;                                             // 0.12f
    init_PIDObject(&yaw_pid, yaw_pid_cfg, 1, -1, 0.005);*/
    
    yaw_assist_pid_cfg.Kp = 25.5f;     //45                                // 0.80f
    yaw_assist_pid_cfg.Ki = 0.05f;     //0.05                            // 0.02f 0.005f
    yaw_assist_pid_cfg.Kd = 0.05f;   // 0.002                                  // 0.12f 0.01
    init_PIDObject(&yaw_assist_pid, yaw_assist_pid_cfg, 150, -150, 0.01);
    
}


/*void AGV_SetMotorOut(short moto_L, short moto_R) {
    
    if(roll>40||roll<-40) {
        PWML=0;
        PWMR=0;
                          }
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
    
    //if((int)PWML<-7000) PWML=-7000;	//限幅
    if(PWML>1000)  PWML=1000;	///7000
		
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
		
    //if((int)PWMR<-7000) PWMR=-7000;	//限幅
    if(PWMR>1000)  PWMR=1000;	
}*/
/*void AGV_360_YawCtrl(float yaw_tgt, float yaw) {  //360
    
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
*/

void AGV_YawCtrl(float yaw_tgt, float yaw) {
    
	float Integral_Temp = yaw_pid.Integral;

	yaw_pid.PreErr = yaw_pid.Err;
	
    // 计算期望    
	yaw_pid.exp  = yaw_tgt;
	yaw_pid.Err  = yaw_pid.exp - yaw;
    
    // 设置一个小的死区
    if (fabs(yaw_pid.Err) <= 5.0f)
        yaw_pid.Err = 0.0f;
    
    // 计算角度
    // 如果角度大于180则采用优弧，这里进行了一些试算，如果需要验证则自行带入数据，如-20->170, 20->-170
    if (fabs(yaw_pid.Err) >= 180.0f) {
        if (yaw_pid.Err >= 0.0f)
            yaw_pid.Err = 360 - yaw_pid.Err;
        else
            yaw_pid.Err = yaw_pid.Err + 360;
    }
        
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

void AGV_VelCtrl(float vel_tgt_l, float vel_tgt_r, float vel_l, float vel_r) {
    
    int out_l, out_r;
    
	/////////////
    //calc_Pid(&vel_pid_l, vel_tgt_l, vel_l);
    //calc_Pid(&vel_pid_r, vel_tgt_r, vel_r);
    calc_Incremental_PI(&vel_pid_l, vel_tgt_l, vel_l);      //  速度闭环必须采用增量式控制，不然会出现严重的卡顿
    calc_Incremental_PI(&vel_pid_r, vel_tgt_r, vel_r);
    
    if(vel_pid_l.output > vel_pid_l_outputMAX) vel_pid_l.output = vel_pid_l_outputMAX;
    if(vel_pid_r.output > vel_pid_r_outputMAX) vel_pid_r.output = vel_pid_r_outputMAX;
    out_l = (int)vel_pid_l.output;
    out_r = (int)vel_pid_r.output;
    
//	if(dst < 20 && dst >500)		AGV_SetMotorOut(0, 0); 		

//	else       //////////7.14
		AGV_SetMotorOut(out_l, out_r); 		

				
          // 这里面已经有幅度限制了，所以这里不再进行输入限幅
    
}



void AGV_Pos_Vel_Ctrl_Straight(float dst_tgt, float dst_current, float vel_l, float vel_r) {

    calc_Pid(&pos_pid, dst_tgt, dst_current);
    AGV_VelCtrl(pos_pid.output, pos_pid.output, vel_l, vel_r);

}

////////////////
void AGV_Pos_Vel_Ctrl_Complex(float x_tgt, float y_tgt, __pos pos_current, float vel_l, float vel_r)
{
    
    float dst_tgt;//
    float yaw_tgt;
    float dst_current;//
    float yaw_current;
    float out_l, out_r;
    
	
    // 计算期望
   dst_tgt = sqrt(x_tgt*x_tgt + y_tgt*y_tgt);//
    yaw_tgt = atan2(y_tgt, x_tgt) * 180.0f / PI;
	
    // 计算当前值
    dst_current = sqrt(pos_current.x*pos_current.x + pos_current.y*pos_current.y);//
    yaw_current = pos_current.yaw;   
   
      
  //  if((dst_tgt-dst_current) < 20)// || (dst_tgt-dst_current) >500)//7.14		
    //AGV_SetMotorOut(0, 0); 
	//AGV_VelCtrl(0, 0, 0, 0); //7.14
    // 变量预设
    out_l = out_r = 0;
    
    // 控制设置优先级，如果角度太大，这样会造成在控制的时候耦合出问题，所以先旋转
    AGV_YawCtrl(yaw_tgt, yaw_current);
    if (fabs(yaw_pid.Err) > 40.0f) {			// 40.0f, 25.0f  
        // 仅角度环，单级PID
        out_l = yaw_pid.output;
        out_r = -yaw_pid.output;
        AGV_SetMotorOut(out_l, out_r);
		                               }
		
    else {
        // 位置-角度外环，速度内环
				//calc_Pid(&pos_pid, dst_tgt, pos_current.x);
			
			 // calc_Pid(&pos_pid, x_tgt, pos_current.x);
        //calc_Pid(&pos_pid, y_tgt, pos_current.y);
			
		calc_Pid(&pos_pid,dst_tgt , dst_current);
			//  calc_Pid_xy(&pos_pid,x_tgt,y_tgt,pos_current.x,pos_current.y);
        calc_Pid(&yaw_assist_pid, yaw_tgt, yaw_current);
        
		if((ang <10 && ang > -10 )|| ang > 360 )yaw_assist_pid.output = 0;
        out_l = pos_pid.output + yaw_assist_pid.output;
			 // if(out_l>10) out_l=10; /////////
			 // if(out_l<-10) out_l=-10;
        out_r = pos_pid.output - yaw_assist_pid.output;
			 // if(out_r>10) out_r=10; /////////
		   //  if(out_r<-10) out_r=-10;
        AGV_VelCtrl(out_l, out_r, vel_l, vel_r);    
				
    }
   
}

void AGV_Tracking_Simple_Spin(float w_exp) {
    
    AGV_YawCtrl(yaw_filtered + w_exp, yaw_filtered);
    AGV_SetMotorOut(yaw_pid.output, -yaw_pid.output);
    
}

void AGV_Tracking_Angle_Dst_Ctrl(float yaw_current, float dst_current) {
    
//    const float yaw_pid_pwm_limit = 1350;
	float out_l, out_r;
	float yaw_tgt;

	yaw_tgt = yaw_current;
	
    calc_Pid(&pos_pid, 20, dst_current);
	calc_Pid(&yaw_assist_pid, 0, yaw_tgt);
       
    
    pos_pid.output = -pos_pid.output;
    if (pos_pid.output < 0)
        pos_pid.output = 0;
    out_l = pos_pid.output;
    out_r = pos_pid.output;
    
        
	AGV_VelCtrl(out_l, out_r, vel_l, vel_r);
	
	if (yaw_assist_pid.output > vel_pid_l.output * 0.75)         // yaw_pid_pwm_limit
		yaw_assist_pid.output = vel_pid_l.output * 0.75;
	else if (yaw_assist_pid.output < -vel_pid_l.output * 0.75)
		yaw_assist_pid.output = -vel_pid_l.output * 0.75;
	
	out_l = (int)vel_pid_l.output + -yaw_assist_pid.output;
	out_r = (int)vel_pid_r.output - -yaw_assist_pid.output;
	AGV_SetMotorOut(out_l, out_r);     
    
}

void AGV_Tracking_Dst_Precise_Control(float yaw_current, float dst_current) {

//  const float yaw_pid_pwm_limit = 1350;
//	const float yaw_err_limit = 35;			// 5
	float out_l, out_r;
	    	
	calc_Pid(&pos_pid, 20, dst_current);

	pos_pid.output = -pos_pid.output;
	if (pos_pid.output < 0)
		pos_pid.output = 0;
	out_l = pos_pid.output;
	out_r = pos_pid.output;
	AGV_VelCtrl(out_l, out_r, vel_l, vel_r);
	
	calc_Pid(&yaw_assist_pid, 0, yaw_current);					 // yaw_current * 100 / dst_current / 2
	if (yaw_assist_pid.output > vel_pid_l.output * 0.45)         // yaw_pid_pwm_limit
		yaw_assist_pid.output = vel_pid_l.output * 0.45;
	else if (yaw_assist_pid.output < -vel_pid_l.output * 0.45)
		yaw_assist_pid.output = -vel_pid_l.output * 0.45;

	
	out_l = (int)vel_pid_l.output * 0.40 + -yaw_assist_pid.output;		// 0.35
	out_r = (int)vel_pid_r.output * 0.40 - -yaw_assist_pid.output;		// 0.35
	AGV_SetMotorOut(out_l, out_r);
        
}
