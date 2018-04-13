#ifndef __EAI_X4_H
#define __EAI_X4_H

#include <rtthread.h>
#include <rthw.h>


extern struct rt_event ControlEvent;


typedef struct
{
    rt_uint8_t  lsn;
    rt_uint16_t fsa;
    rt_uint16_t lsa;
    rt_uint16_t cs;
    rt_uint16_t buf[40];
} PackAge;


typedef struct
{
    rt_uint16_t angle;
    rt_uint16_t distance;
} AroundPoint;

typedef struct
{
    AroundPoint firstpont;
    AroundPoint ap[1440];
		rt_int16_t number;
} Around;

extern Around around[5];
//extern rt_int16_t number[5];

void EAI_X4_GetDate(void);

#endif

