#include "include/odom.h"

__pos pos_current, pos_last;                    // 位姿向量,           cm, deg
float dst_l, dst_r, dst_agv, dst_agv_last;      // 车轮走过的路程,     cm
float vel_l, vel_r, vel_l_last, vel_r_last;     // 左右两个车轮的速度, cm/s
float vel_agv, vel_agv_last;

float pitch,  roll,  yaw;                  	    // 欧拉角
float acc_x,  acc_y, acc_z;                     // 加速度数据
float gyro_x, gyro_y, gyro_z;	                // 陀螺仪数据
float yaw_encoder, yaw_filtered;

float dt_sample_ms;

bool is_imu_set;
//bool is_dst_set;

void  init_Odom_Variables(void) {
    
    pos_current.x = pos_current.y = pos_current.yaw = 0.0f;
    pos_last = pos_current;
    
    dst_l = dst_r = dst_agv = dst_agv_last = 0.0f;
    vel_l = vel_r = 0.0f;
    vel_l_last = vel_r_last = 0.0f;
    
    yaw_encoder = 0.0f;
    
    dt_sample_ms = 10.0f;
}

void  update_Odom_IMU(void) {
  
    const float yaw_k1 = 0.667f;
    const float yaw_k2 = 1.0f - yaw_k1;
    
    static u8 i = 0;
    
    // 更新IMU, 200Hz
    if (i == 0)
        is_imu_set = update_IMU_Data();
    
    // 更新里程计，100Hz
    if (i % 1 == 0) {
        update_Dst_LR(encoder_left, encoder_right);
        update_Vel_LR(encoder_left, encoder_right);
        update_Yaw(dst_l, dst_r);
    
        // 偏航角互补滤波
        yaw_filtered = yaw_k1 * yaw_encoder + yaw_k2 * yaw;
        
        // 更新位姿向量
        update_LocationVector(dst_agv, dst_agv_last, yaw_filtered);
    }
    
    i++;
    if (i >= 2)
        i = 0;
}

float update_Dst_LR(int encoder_l, int encoder_r) {
    
    // 左右车轮速度积分，得到位移
    float dst_car_temp;
    float l_ang_temp, r_ang_temp;
    
    l_ang_temp = encoder_l * dt_sample_ms / 1000.0f;                                            // 计算轮子单次转了几圈
    r_ang_temp = encoder_r * dt_sample_ms / 1000.0f;
    
    dst_l += l_ang_temp / (float)AGV_Encoder_Val_perRound * PI * AGV_Tire_Diametre;             // 角度 * PI * 直径 = 路程
    dst_r += r_ang_temp / (float)AGV_Encoder_Val_perRound * PI * AGV_Tire_Diametre;
    
    dst_car_temp = (dst_l + dst_r) / 2.0f;
 
    dst_agv_last = dst_agv;                                                                     // 更新路程
    dst_agv      = dst_car_temp;
    return dst_car_temp;
}

float update_Vel_LR(int encoder_l, int encoder_r) {
    
    // 编码器角位移换速度
    const float k1 = 0.55f;
    const float k2 = 1.0f - k1;                                                                 // LPF参数
    
    float l_rvel_temp, r_rvel_temp;
    float vel_l_temp,  vel_r_temp;
    
    l_rvel_temp = (float)encoder_l / (float)AGV_Encoder_Val_perRound * 2. * PI;                 // 一圈走了多少 * 2 * PI = 角速度
    r_rvel_temp = (float)encoder_r / (float)AGV_Encoder_Val_perRound * 2. * PI;
    
    vel_l_temp = l_rvel_temp * AGV_Tire_Diametre / 2.0f;                                        // v = w * r，角速度换速度
    vel_r_temp = r_rvel_temp * AGV_Tire_Diametre / 2.0f;
    
    vel_l_last = vel_l;
    vel_r_last = vel_r;
    vel_l = vel_l_temp;
    vel_r = vel_r_temp;                                                                         // 更新数据
    
    vel_l = k1 * vel_l + k2 * vel_l_last;
    vel_r = k1 * vel_r + k2 * vel_r_last;                                                       // LPF
    
    vel_agv_last = vel_agv;
    vel_agv = (vel_l + vel_r) / 2.0f;
    return vel_agv;
    
}

float update_Yaw(float dst_l, float dst_r) {
    
    float ds_lr;
    
    ds_lr = (dst_l - dst_r) / 2.0f;
    yaw_encoder = ds_lr / (AGV_Width / 2);                  // 两边轮子测差速除以圆周率再除以车轴长度就是转过的角度, 周长=弧长*R
    yaw_encoder *= 180.0f / PI;                             // 弧度->角度
    
    while (yaw_encoder >= 180.0f)                           // 角度归一化
        yaw_encoder -= 360.0f;
    while (yaw_encoder <  -180.0f)
        yaw_encoder += 360.0f;
    
    return yaw_encoder;
}

void update_LocationVector(float dst_agv, float dst_agv_last, float yaw_deg)
{
    
    float ds;
    float dx, dy;
    
    ds = dst_agv - dst_agv_last;///////
    dx = ds * cos(yaw_deg * PI / 180.0f);
    dy = ds * sin(yaw_deg * PI / 180.0f);
    
    pos_last = pos_current;
    pos_current.x += dx;
    pos_current.y += dy; 
    pos_current.yaw = yaw_deg;
    
}

bool update_IMU_Data(void) {
    
    const float gyro_range = 2000.0f;                               // 陀螺仪量程，见mpu6050.c， degree per second
    
    short gx_raw, gy_raw, gz_raw;
    static float gz_pre = 0.0f, gz_last = 0.0f, gz_now = 0.0f;

    //static float yaw_now = 0.0f, yaw_pre = 0.0f, yaw_last = 0.0f; // 偏航角不能均值，否则影响积分
    
    bool stat_euler = false;
    bool stat_gyro  = false;
       
    
    if (mpu_dmp_get_data(&pitch ,&roll ,&yaw) == 0)
        stat_euler = true;
    else
        stat_euler = false;
    
    if (stat_euler) {
              
        while (yaw >= 180.0f)                                       // 偏航角归一化
            yaw -= 360.0f;
        while (yaw <  -180.0f)
            yaw += 360.0f;
        yaw = -yaw;       
    }
    
    if (MPU_Get_Gyroscope(&gx_raw,&gy_raw,&gz_raw) == 0)
        stat_gyro = true;
    else
        stat_gyro = false;
    
    if (stat_gyro) {
        gyro_x = (float)gx_raw / 32768.0f * gyro_range;              // 虚值转换为实际值
        gyro_y = (float)gy_raw / 32768.0f * gyro_range;
        
        gz_last = gz_pre;
        gz_pre  = gz_now;
        gz_now = (float)gz_raw / 32768.0f * gyro_range;              // 滑动均值滤波
        
        gyro_z = (gz_now + gz_pre + gz_last) / 3.0f;
        gyro_z = -gyro_z;
    }
       
    return stat_euler && stat_gyro;
}

