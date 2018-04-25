#include <rthw.h>
#include <rtdevice.h>

#include "drv_abphase.h"
#include "app_speed.h"
#include "stm32f4xx_hal.h"
void speed_thread_entry(void *parameter)
{
	rt_uint32_t speed1;
	rt_uint32_t speed2;
    while (1)
    {
			getspeedcounter(&speed1,&speed2);
			setcounterzero();
			rt_kprintf("speed1:%d , speed2:%d \n",speed1,speed2);
			rt_thread_delay(rt_tick_from_millisecond(200));
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
                           3,
                           10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
INIT_APP_EXPORT(speed_init);
