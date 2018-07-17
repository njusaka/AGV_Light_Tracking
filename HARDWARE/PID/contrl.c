//#include "contrl.h"
//#define fil 0.01 //低通滤波系数
////速度增量PI控制  速度环输出=比例系数P*速度误差+积分系数I*积分值。
//int velocity_Pi(int encoder_lef,int encoder_rig,int encoder_target)
//{
//	static float Velocity,Error,Error_I,Error_fil;	
//	Error          =  (encoder_lef+encoder_rig)-0;  //测量速度-目标速度=误差
//	Error_fil      =   Error*fil+Error*(1-fil);                    //一阶滤波
//	Error_I       +=   Error_fil;   
//	if(Error_I >80000)Error_I=80000;
//	if(Error_I <-80000)Error_I=-80000;
//	Velocity       =   Error_fil*Velocity_Kp + Error_I *Velocity_Ki;
//	return Velocity;

//}//需要加上输出限值



////角度闭环PD控制  直立环输出=比例系数P*倾角误差+微分系数D*角速度
//int location_Pd(float Roll, double Gyrox, float Location_angle)
//{
//	float Angle;
//	int Location;
//	Angle       = Roll - Location_angle;
//	Location    = Location_Kp*Angle + Gyrox*Location_Kd;
//	return Location;
//}

////转向环控制  转向环输出=比例系数p*z轴角速度
//int turn_P(int encoder_lef,int encoder_rig,float gyroz)
//{

//		int turn;

//		turn=-gyroz*Turn_Kd+turn_target;
//		return turn;
//}


