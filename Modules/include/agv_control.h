#ifndef __AGV_Control_H
#define __AGV_Control_H


#include <stm32f10x.h>
#include <config.h>

#include <PID.h>

#include <motor.h>

#include "odom.h"


void AGV_SetMotorOut(short moto_L, short moto_R);

void AGV_PIDCtrlInit(void);
void AGV_YawCtrl(float yaw_tgt, float yaw);
void AGV_VelCtrl(float vel_tgt_l, float vel_tgt_r, float vel_l, float vel_r);
void AGV_Pos_Vel_Ctrl_Straight(float dst_tgt, float dst_current, float vel_l, float vel_r);
void AGV_Pos_Vel_Ctrl_Complex(float x_tgt, float y_tgt, __pos pos_current, float vel_l, float vel_r);
void AGV_Tracking_Simple_Spin(float w_exp);
void AGV_Tracking_Angle_Dst_Ctrl(float yaw_current, float dst_current);
void AGV_Tracking_Dst_Precise_Control(float yaw_current, float dst_current);


extern __pid yaw_pid;
extern __pid pos_pid;
extern __pid vel_pid_l, vel_pid_r;
extern float yaw_current;


#endif  // __AGV_Control_H

