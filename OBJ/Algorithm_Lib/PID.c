#include "PID.h"
#include "math.h"

void init_PIDObject(__pid *pid_in,__pid_cfg pid_cfg,float integral_upper,float integral_lower,float dt_s){
	pid_in->exp = 0;
	pid_in->Err = 0;
	pid_in->PreErr = 0;
	pid_in->Integral = 0;
	pid_in->Derivative = 0;
	pid_in->Kp = pid_cfg.Kp;
	pid_in->Ki = pid_cfg.Ki;
	pid_in->Kd = pid_cfg.Kd;
	pid_in->Pout = 0;
	pid_in->Iout = 0;
	pid_in->Dout = 0;
	pid_in->output = 0;
	pid_in->IntegralUpper = integral_upper;
	pid_in->IntegralLower = integral_lower;
	pid_in->dt = dt_s;
}

/////////////
void calc_Pid(__pid *pid,float exp,float data_now)
{
	float Integral_Temp = pid->Integral;

	pid->PreErr = pid->Err;
	
	pid->exp  = exp;
	pid->Err  = pid->exp - data_now;
	pid->Pout = pid->Kp * pid->Err;
	
	Integral_Temp += pid->Err * pid->dt;
	//pid->Integral = limit_Data(Integral_Temp, pid->IntegralUpper, pid->IntegralLower);
	pid->Iout = pid->Ki * Integral_Temp;
	pid->Iout = limit_Data(pid->Iout, pid->IntegralUpper, pid->IntegralLower);
    
	pid->Derivative = (pid->Err - pid->PreErr) / pid->dt;
	pid->Dout = pid->Kd * pid->Derivative;
	
	pid->output = pid->Pout + pid->Iout + pid->Dout;	
}

	/*void calc_Pid_xy(__pid *pid,float exp_x,float exp_y,float data_now_x,float data_now_y){
	
	float Integral_Temp = pid->Integral;
	float data_exp_now;
	
	pid->PreErr = pid->Err;
	 
	  
data_exp_now=sqrt((exp_x-data_now_x)*(exp_x-data_now_x) + (exp_y-data_now_y)*(exp_y-data_now_y));
	if()
		
	if()
	pid->Err  = data_exp_now;
	pid->Pout = pid->Kp * pid->Err;//
	
	Integral_Temp += pid->Err * pid->dt;
	//pid->Integral = limit_Data(Integral_Temp, pid->IntegralUpper, pid->IntegralLower);
	pid->Iout = pid->Ki * Integral_Temp;//
	pid->Iout = limit_Data(pid->Iout, pid->IntegralUpper, pid->IntegralLower);
    
	pid->Derivative = (pid->Err - pid->PreErr) / pid->dt;
	pid->Dout = pid->Kd * pid->Derivative;//
	
	pid->output = pid->Pout + pid->Iout + pid->Dout;	
}
*/

 /* void calc_Pid(__pid *pid,float exp,float data_now)
{
	float Integral_Temp = pid->Integral;

	pid->PreErr = pid->Err;
	
	pid->exp  = exp;
	pid->Err  = pid->exp - data_now;
	pid->Pout = pid->Kp * pid->Err;

	Integral_Temp += pid->Err * pid->dt;
	//pid->Integral = limit_Data(Integral_Temp, pid->IntegralUpper, pid->IntegralLower);
	pid->Iout = pid->Ki * Integral_Temp;
	pid->Iout = limit_Data(pid->Iout, pid->IntegralUpper, pid->IntegralLower);
    
	pid->Derivative = (pid->Err - pid->PreErr) / pid->dt;
	pid->Dout = pid->Kd * pid->Derivative;
	
	pid->output = pid->Pout + pid->Iout + pid->Dout;	
}*/


//原pid
void calc_Incremental_PI(__pid *pid, float exp, float data_now)
{
    
    // 增量式PI控制器计算
    pid->PreErr = pid->Err;
    pid->exp    = exp;
    pid->Err    = exp - data_now;
    
    // P
    pid->Pout = pid->Kp * (pid->Err - pid->PreErr);
    // I
    pid->Iout = pid->Ki * pid->Err;
   
    // 输出
    pid->output += pid->Pout + pid->Iout;       
}

float limit_Data(float in,float Upper,float Lower)
{
	if(Upper != Lower){
		if(in >= Upper)
			return Upper;
		else if(in <= Lower)
			return Lower;
	}
	return in;
}
