#ifndef __PID_H
#define __PID_H

#include <config.h>
#include "math.h"



typedef struct{

	float exp;
//double exp;//
	float Err;
	float PreErr;
	float Integral;
	float Derivative;
	float Kp;
	float Ki;
	float Kd;
	float Pout;
	float Iout;
	float Dout;
	float output;
	float IntegralUpper;
	float IntegralLower;//IntegralUpper == IntegralLower means no limit;
	float dt; 
	
	float data_exp_now;//
} __pid;




typedef struct{
	float Kp;
	float Ki;
	float Kd;
} __pid_cfg;

void init_PIDObject(__pid *pid_in,__pid_cfg pid_cfg,float integral_upper,float integral_lower,float dt_s);
void calc_Pid(__pid *pid,float exp,float data_now);
void calc_Incremental_PI(__pid *pid, float exp, float data_now);
float limit_Data(float in,float Upper,float Lower);
void calc_Pid_xy(__pid *pid,float exp_x,float exp_y,float data_now_x,float data_now_y);

#endif

