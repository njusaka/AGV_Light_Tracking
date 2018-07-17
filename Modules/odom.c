#include "include/odom.h"

__pos pos_current, pos_last;                    // λ������,           cm, deg
float dst_l, dst_r, dst_agv, dst_agv_last;      // �����߹���·��,     cm
float vel_l, vel_r, vel_l_last, vel_r_last;     // �����������ֵ��ٶ�, cm/s
float vel_agv, vel_agv_last;

float pitch,  roll,  yaw;                  	    // ŷ����
float acc_x,  acc_y, acc_z;                     // ���ٶ�����
float gyro_x, gyro_y, gyro_z;	                // ����������
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
    
    // ����IMU, 200Hz
    if (i == 0)
        is_imu_set = update_IMU_Data();
    
    // ������̼ƣ�100Hz
    if (i % 1 == 0) {
        update_Dst_LR(encoder_left, encoder_right);
        update_Vel_LR(encoder_left, encoder_right);
        update_Yaw(dst_l, dst_r);
    
        // ƫ���ǻ����˲�
        yaw_filtered = yaw_k1 * yaw_encoder + yaw_k2 * yaw;
        
        // ����λ������
        update_LocationVector(dst_agv, dst_agv_last, yaw_filtered);
    }
    
    i++;
    if (i >= 2)
        i = 0;
}

float update_Dst_LR(int encoder_l, int encoder_r) {
    
    // ���ҳ����ٶȻ��֣��õ�λ��
    float dst_car_temp;
    float l_ang_temp, r_ang_temp;
    
    l_ang_temp = encoder_l * dt_sample_ms / 1000.0f;                                            // �������ӵ���ת�˼�Ȧ
    r_ang_temp = encoder_r * dt_sample_ms / 1000.0f;
    
    dst_l += l_ang_temp / (float)AGV_Encoder_Val_perRound * PI * AGV_Tire_Diametre;             // �Ƕ� * PI * ֱ�� = ·��
    dst_r += r_ang_temp / (float)AGV_Encoder_Val_perRound * PI * AGV_Tire_Diametre;
    
    dst_car_temp = (dst_l + dst_r) / 2.0f;
 
    dst_agv_last = dst_agv;                                                                     // ����·��
    dst_agv      = dst_car_temp;
    return dst_car_temp;
}

float update_Vel_LR(int encoder_l, int encoder_r) {
    
    // ��������λ�ƻ��ٶ�
    const float k1 = 0.55f;
    const float k2 = 1.0f - k1;                                                                 // LPF����
    
    float l_rvel_temp, r_rvel_temp;
    float vel_l_temp,  vel_r_temp;
    
    l_rvel_temp = (float)encoder_l / (float)AGV_Encoder_Val_perRound * 2. * PI;                 // һȦ���˶��� * 2 * PI = ���ٶ�
    r_rvel_temp = (float)encoder_r / (float)AGV_Encoder_Val_perRound * 2. * PI;
    
    vel_l_temp = l_rvel_temp * AGV_Tire_Diametre / 2.0f;                                        // v = w * r�����ٶȻ��ٶ�
    vel_r_temp = r_rvel_temp * AGV_Tire_Diametre / 2.0f;
    
    vel_l_last = vel_l;
    vel_r_last = vel_r;
    vel_l = vel_l_temp;
    vel_r = vel_r_temp;                                                                         // ��������
    
    vel_l = k1 * vel_l + k2 * vel_l_last;
    vel_r = k1 * vel_r + k2 * vel_r_last;                                                       // LPF
    
    vel_agv_last = vel_agv;
    vel_agv = (vel_l + vel_r) / 2.0f;
    return vel_agv;
    
}

float update_Yaw(float dst_l, float dst_r) {
    
    float ds_lr;
    
    ds_lr = (dst_l - dst_r) / 2.0f;
    yaw_encoder = ds_lr / (AGV_Width / 2);                  // �������Ӳ���ٳ���Բ�����ٳ��Գ��᳤�Ⱦ���ת���ĽǶ�, �ܳ�=����*R
    yaw_encoder *= 180.0f / PI;                             // ����->�Ƕ�
    
    while (yaw_encoder >= 180.0f)                           // �Ƕȹ�һ��
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
    
    const float gyro_range = 2000.0f;                               // ���������̣���mpu6050.c�� degree per second
    
    short gx_raw, gy_raw, gz_raw;
    static float gz_pre = 0.0f, gz_last = 0.0f, gz_now = 0.0f;

    //static float yaw_now = 0.0f, yaw_pre = 0.0f, yaw_last = 0.0f; // ƫ���ǲ��ܾ�ֵ������Ӱ�����
    
    bool stat_euler = false;
    bool stat_gyro  = false;
       
    
    if (mpu_dmp_get_data(&pitch ,&roll ,&yaw) == 0)
        stat_euler = true;
    else
        stat_euler = false;
    
    if (stat_euler) {
              
        while (yaw >= 180.0f)                                       // ƫ���ǹ�һ��
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
        gyro_x = (float)gx_raw / 32768.0f * gyro_range;              // ��ֵת��Ϊʵ��ֵ
        gyro_y = (float)gy_raw / 32768.0f * gyro_range;
        
        gz_last = gz_pre;
        gz_pre  = gz_now;
        gz_now = (float)gz_raw / 32768.0f * gyro_range;              // ������ֵ�˲�
        
        gyro_z = (gz_now + gz_pre + gz_last) / 3.0f;
        gyro_z = -gyro_z;
    }
       
    return stat_euler && stat_gyro;
}
