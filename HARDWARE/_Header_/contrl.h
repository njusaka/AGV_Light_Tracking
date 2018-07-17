#ifndef __CONTRL_H
#define __CONTRL_H
#include "sys.h"
#include "car.h"

typedef struct 
{
int setpoint;//�趨Ŀ��
int sum_error;//����ۼ�
float proportion ;//��������
float integral ;//���ֳ���
float derivative;//΢�ֳ���
int last_error;//e[-1]
int prev_error;//e[-2]
}PID;

int velocity_Pi(int encoder_lef,int encoder_rig,int encoder_target);
int location_Pd(float Roll, double Gyrox, float set_angle);
int turn_P(int encoder_lef,int encoder_rig,float gyroz); 
#endif
