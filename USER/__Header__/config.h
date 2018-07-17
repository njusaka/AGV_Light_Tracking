#ifndef __AGV_Config_H
#define __AGV_Config_H

//#include <stm32f10x.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char   bool;
#define true            1
#define false           0
  

#define PI              3.1415926535897932384626f

typedef struct {
    float x;
    float y;
    float yaw;
} __pos;

typedef struct {
    
    float x;
    float y;
    
} __vec2f;

typedef enum {    
    moving    = 1,
    stopped   = 2,
    light_off = 3,
} __mission_status;

#endif  // __AGV_Config_H
