#ifndef __PID_H
#define __PID_H

#include <config.h>

typedef struct{
	float exp;
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

#endif

