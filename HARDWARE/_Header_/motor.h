#ifndef __Motor_H
#define __Motor_H

#define moto_R_1 PBout(5)
#define moto_R_2 PAout(15)
#define moto_L_1 PAout(12)
#define moto_L_2 PBout(4)

#define PWMR   TIM1->CCR4  
#define PWML   TIM1->CCR1  
#define Dead_zone 450

void Motor_Init(void);
void PWM_Init(int arr, int psc);
void Set_motor(int moto1, int moto2);
void trac_gpio_config(void);
#endif /*__Motor_H */
