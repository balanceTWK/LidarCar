#include <rthw.h>
#include <rtthread.h>

#include "app_beep.h"
#include <drivers/pin.h>

#define BEEP 167 //PB8--167£¬ÔÚ drv_gpio.c ÎÄ¼þ pin_index pins[]

void beep(rt_uint8_t i)
{
		if(i==1)
		{
			rt_pin_write(BEEP, PIN_HIGH);
		}
		else 
		{
			rt_pin_write(BEEP, PIN_LOW);
		}
}

int beep_init(void)
{
		rt_pin_mode(BEEP, PIN_MODE_OUTPUT);
		
    return 0;
}
INIT_APP_EXPORT(beep_init);

