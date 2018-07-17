#ifndef __Data_H_
#define __Data_H_

#include "car.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x_it.h"
#define Rx_Len_Minium_Len 15


char refine_Data(char rx[], int len, float *ang, float *dst);

#endif /*__Data_H*/
