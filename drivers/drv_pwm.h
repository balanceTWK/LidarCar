#ifndef __PWM_H__
#define __PWM_H__

#include <rthw.h>
#include <rtthread.h>

void SetTim9Compare(rt_uint32_t compare1,rt_uint32_t compare2);
void SetTim2Compare(rt_uint32_t compare);

#endif
