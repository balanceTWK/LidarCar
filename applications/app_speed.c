#include <rthw.h>
#include <rtdevice.h>

#include "drv_abphase.h"
#include "drv_pwm.h"
#include "app_speed.h"
#include "stm32f4xx_hal.h"

#include "master_thread.h"

void speed_thread_entry(void *parameter)
{
    rt_int16_t speed1;
    rt_int16_t speed2;
	
//    rt_uint16_t xspeed1 = 600;
//    rt_uint16_t xspeed2 = 600;
	
		rt_int16_t diff1;
		rt_int16_t diff2;
	
    rt_int16_t pwm1;
    rt_int16_t pwm2;

    while (1)
    {
        getspeedcounter(&speed1, &speed2);
        setcounterzero();
//				if(rt_mutex_take(&SpeedMutex,RT_WAITING_NO) == RT_EOK)
//				{
//					xspeed1=wantspeed1;
//					xspeed2=wantspeed2;
//					rt_mutex_release(&SpeedMutex);
//				}
			
				diff1=wantspeed1-speed1;
				diff2=wantspeed2-speed2;
			
				pwm1 = pwm1 - diff1;
				pwm2 = pwm2 - diff2;
			
				if(pwm1<0)
				{
					pwm1=0;
				}
				if(pwm2<0)
				{
					pwm2=0;
				}
				
				SetTim9Compare(pwm1,pwm2);
				
        rt_kprintf("speed1:%d , speed2:%d \n", speed1, speed2);
        rt_thread_delay(rt_tick_from_millisecond(50));
    }
}

int speed_init()
{
    rt_thread_t tid;

    /* 创建test线程 */
    tid = rt_thread_create("speed",
                           speed_thread_entry,
                           RT_NULL,
                           1024,
                           5,
                           10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
