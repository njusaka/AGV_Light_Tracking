//#include "contrl.h"
//#define fil 0.01 //��ͨ�˲�ϵ��
////�ٶ�����PI����  �ٶȻ����=����ϵ��P*�ٶ����+����ϵ��I*����ֵ��
//int velocity_Pi(int encoder_lef,int encoder_rig,int encoder_target)
//{
//	static float Velocity,Error,Error_I,Error_fil;	
//	Error          =  (encoder_lef+encoder_rig)-0;  //�����ٶ�-Ŀ���ٶ�=���
//	Error_fil      =   Error*fil+Error*(1-fil);                    //һ���˲�
//	Error_I       +=   Error_fil;   
//	if(Error_I >80000)Error_I=80000;
//	if(Error_I <-80000)Error_I=-80000;
//	Velocity       =   Error_fil*Velocity_Kp + Error_I *Velocity_Ki;
//	return Velocity;

//}//��Ҫ���������ֵ



////�Ƕȱջ�PD����  ֱ�������=����ϵ��P*������+΢��ϵ��D*���ٶ�
//int location_Pd(float Roll, double Gyrox, float Location_angle)
//{
//	float Angle;
//	int Location;
//	Angle       = Roll - Location_angle;
//	Location    = Location_Kp*Angle + Gyrox*Location_Kd;
//	return Location;
//}

////ת�򻷿���  ת�����=����ϵ��p*z����ٶ�
//int turn_P(int encoder_lef,int encoder_rig,float gyroz)
//{

//		int turn;

//		turn=-gyroz*Turn_Kd+turn_target;
//		return turn;
//}


