#ifndef __MASTER_H
#define __MASTER_H

#include <rtthread.h>
#include <rthw.h>

#define controlEventEaix4 (1 << 0)

extern struct rt_event ControlEvent;
extern struct rt_mutex SpeedMutex;
extern struct rt_mutex Mpu9250Mutex;

extern volatile float mypitch, myroll, myyaw;

extern rt_int16_t wantspeed1;
extern rt_int16_t wantspeed2;

typedef struct
{
    rt_uint16_t fsa;
    rt_uint16_t lsa;
} Blank;

void master_thread_entry(void *parameter);

#endif




