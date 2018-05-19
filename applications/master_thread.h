#ifndef __MASTER_H
#define __MASTER_H

#include <rtthread.h>
#include <rthw.h>

#define controlEventEaix4 (1 << 0)
#define control1 (1 << 1)
#define control2 (1 << 2)
#define control3 (1 << 3)

extern struct rt_event ControlEvent;
extern struct rt_mutex SpeedMutex;
extern struct rt_mutex Mpu9250Mutex;
extern struct rt_mutex StatusMutex;
extern struct rt_mutex DistanceMutex;

extern volatile float mypitch, myroll, myyaw;

extern volatile rt_int16_t wantspeed1;
extern volatile rt_int16_t wantspeed2;
extern volatile rt_uint32_t distance;
extern volatile rt_uint32_t CarStatus;

typedef struct
{
    rt_uint16_t fsa;
    rt_uint16_t lsa;
} Blank;

void master_thread_entry(void *parameter);

#endif




