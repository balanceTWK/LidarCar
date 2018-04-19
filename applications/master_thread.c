#include "master_thread.h"
#include "app_eaix4.h"
#include "app_wirelessuart.h"
#include "app_mpu9250.h"
#include "app_beep.h"
#include "EAI_X4.h"
#include "app_move.h"
struct rt_event ControlEvent;

void master_thread_entry(void *parameter)
{
    rt_uint32_t e;
    rt_uint8_t i;
		rt_uint32_t buf;
		rt_int8_t fsignal=0;
		rt_int8_t bsignal=0;
		rt_int8_t lsignal=0;
		rt_int8_t rsignal=0;

    while (1)
    {
        /* 接收事件 */
        if (rt_event_recv(&ControlEvent, controlEventEaix4, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, &e) == RT_EOK)
        {
            for (i = 1; i < 5; i++)
            {
                for (rt_uint16_t x = 0; x < around[i].number; x++)
                {
                    if (around[i].ap[x].distance <= 800)
                    {
//                      rt_kprintf("warning circle:%d angle:%d distance:%d\n", i, ((rt_uint32_t)around[i].ap[x].angle) * 100 / 64, ((rt_uint32_t)around[i].ap[x].distance) / 4);
											buf = (rt_uint32_t)around[i].ap[x].angle * 100 / 64 ;
											if(buf >= 31500 || buf < 4500)
											{
												fsignal++;
											}
											else if(buf >= 4500 && buf < 13500)
											{
												rsignal++;
											}
											else if(buf >= 13500 && buf < 22500)
											{
												bsignal++;
											}
											else if(buf >= 22500 && buf < 31500)
											{
												lsignal++;
											}											
                    }										
                }	
            }
								if(fsignal>=3)
								{
									carBack();
								}
								else if(rsignal>=3)
								{
									carLeft();
								}
								else if(bsignal>=3)
								{
									carForward();
								}
								else if(lsignal>=3)
								{
									carRight();
								}
								else
								{
									carStop();
								}
							 fsignal=0;
							 bsignal=0;
							 lsignal=0;
							 rsignal=0;
//            beep(1);
//            rt_thread_delay(rt_tick_from_millisecond(50));
//            beep(0);
            Eaix4Scaning();
        }
        else
        {
            rt_thread_delay(rt_tick_from_millisecond(50));
        }
    }
}

int master_init()
{
    rt_thread_t tid;
    rt_kprintf("master_init.\n");
    rt_event_init(&ControlEvent, "Control", RT_IPC_FLAG_FIFO);
    eaix4_init();
    wireless_init();
    mpu9250_init();
    rt_kprintf("master_done.\n");
    /* 创建test线程 */
    tid = rt_thread_create("master",
                           master_thread_entry,
                           RT_NULL,
                           2048,
                           3,
                           20);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
INIT_APP_EXPORT(master_init);
