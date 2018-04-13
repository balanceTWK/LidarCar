#ifndef __MASTER_H
#define __MASTER_H

#include <rtthread.h>
#include <rthw.h>

#define controlEventEaix4 (1 << 0)
extern struct rt_event ControlEvent;
void master_thread_entry(void *parameter);
#endif




