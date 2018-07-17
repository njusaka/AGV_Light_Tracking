#ifndef __SERVO_H
#define __SERVO_H	  

void TIM3_Int_Init(int arr,int psc);
void TIM3_PWM_Init(int arr,int psc);
void set_Servo_Angles(float angle);


#endif
