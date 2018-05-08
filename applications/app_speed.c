#include <rthw.h>
#include <rtdevice.h>

#include "drv_abphase.h"
#include "drv_pwm.h"
#include "app_speed.h"
#include "app_move.h"
#include "stm32f4xx_hal.h"

#include "master_thread.h"

void setpwm(rt_int16_t pwm1, rt_int16_t pwm2)
{
    if (pwm1 >= 0)
    {
        rt_pin_write(155, PIN_LOW);
        rt_pin_write(154, PIN_HIGH);
    }
    else
    {
        rt_pin_write(155, PIN_HIGH);
        rt_pin_write(154, PIN_LOW);
        pwm1 = -pwm1;
    }

    if (pwm2 >= 0)
    {
        rt_pin_write(156, PIN_LOW);
        rt_pin_write(157, PIN_HIGH);
    }
    else
    {
        rt_pin_write(156, PIN_HIGH);
        rt_pin_write(157, PIN_LOW);
        pwm2 = -pwm2;
    }
    SetTim9Compare(pwm1, pwm2);
}

void speed_thread_entry(void *parameter)//PID控制
{
    rt_int16_t speed1;
    rt_int16_t speed2;

    rt_int16_t xspeed1 = 0;
    rt_int16_t xspeed2 = 0;

    rt_int16_t diff1;
    rt_int16_t diff2;

    rt_int16_t Last_diff1 = 0;
    rt_int16_t Last_diff2 = 0;

    rt_int16_t pwm1 = 0;
    rt_int16_t pwm2 = 0;
	
		rt_uint8_t s;
    //////////////////////
//      carForward();//测试
    //////////////////////
    while (1)
    {
        getspeedcounter(&speed1, &speed2);
        setcounterzero();
        if (rt_mutex_take(&SpeedMutex, RT_WAITING_NO) == RT_EOK)
        {
            xspeed1 = wantspeed1;
            xspeed2 = wantspeed2;
            if ((xspeed1 == 0) && (xspeed2 == 0))
            {
                pwm1 = 0;
                pwm2 = 0;
                goto out;
            }
        }
        diff1 = xspeed1 - speed1;
        diff2 = xspeed2 - speed2;
        pwm1 = pwm1 + 6 * diff1 + 5 * (diff1 - Last_diff1);
        pwm2 = pwm2 + 6 * diff2 + 5 * (diff2 - Last_diff2);
        Last_diff1 = diff1;
        Last_diff2 = diff2;
out:
        setpwm(pwm1, pwm2);
				rt_mutex_take(&StatusMutex, RT_WAITING_FOREVER);
				s=CarStatus&0x00000007;
				rt_mutex_release(&StatusMutex);
				if((s==1)||(s==2))
				{
					rt_mutex_take(&DistanceMutex, RT_WAITING_FOREVER);
					distance+=(speed1+speed2)/2;
					rt_mutex_release(&DistanceMutex);
				}
				
//        rt_kprintf("speed1:%d , speed2:%d  ",speed1, speed2);
//              rt_kprintf("pwm1:%d , pwm2:%d \n", pwm1, pwm2);
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
                           256,
                           3,
                           10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
