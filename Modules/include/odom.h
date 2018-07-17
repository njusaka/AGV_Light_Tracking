#ifndef __Odom_H
#define __Odom_H

#include <config.h>
#include <agv_param.h>
#include <stm32f10x.h>

#include <mpu6050.h>
#include <inv_mpu.h>


void  init_Odom_Variables(void);
void  update_Odom_IMU(void);

float update_Dst_LR(int encoder_l, int encoder_r);
float update_Vel_LR(int encoder_l, int encoder_r);
float update_Yaw(float dst_l, float dst_r);

void update_LocationVector(float dst_agv, float dst_agv_last, float yaw_deg);
bool update_IMU_Data(void);

extern __pos pos_current, pos_last;                    // λ������,           cm, deg
extern float dst_l, dst_r, dst_agv, dst_agv_last;      // �����߹���·��,     cm
extern float vel_l, vel_r, vel_l_last, vel_r_last;     // �����������ֵ��ٶ�, cm/s
extern float vel_agv, vel_agv_last;

extern float pitch,  roll,  yaw;                       // ŷ����
extern float acc_x,  acc_y, acc_z;                     // ���ٶ�����
extern float gyro_x, gyro_y, gyro_z;                   // ����������
extern float yaw_encoder, yaw_filtered;

extern bool is_imu_set;

#endif  // __Odom_H

