#ifndef __MASTER_H
#define __MASTER_H

#include <rtthread.h>
#include <rthw.h>

#define controlEventEaix4 (1 << 0)

extern struct rt_event ControlEvent;
extern struct rt_mutex SpeedMutex;

extern rt_int16_t wantspeed1;
extern rt_int16_t wantspeed2;

void master_thread_entry(void *parameter);

#endif




