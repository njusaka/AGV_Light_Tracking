#ifndef __CONTRL_H
#define __CONTRL_H
#include "sys.h"
#include "car.h"

typedef struct 
{
int setpoint;//设定目标
int sum_error;//误差累计
float proportion ;//比例常数
float integral ;//积分常数
float derivative;//微分常数
int last_error;//e[-1]
int prev_error;//e[-2]
}PID;

int velocity_Pi(int encoder_lef,int encoder_rig,int encoder_target);
int location_Pd(float Roll, double Gyrox, float set_angle);
int turn_P(int encoder_lef,int encoder_rig,float gyroz); 
#endif
