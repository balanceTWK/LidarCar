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
    rt_int8_t fsignalL = 0;
    rt_int8_t fsignalR = 0;
    rt_int8_t bsignalL = 0;
    rt_int8_t bsignalR = 0;
    rt_int8_t lsignalL = 0;
    rt_int8_t lsignalR = 0;
    rt_int8_t rsignalL = 0;
    rt_int8_t rsignalR = 0;

    while (1)
    {
        /* 接收事件 */
        if (rt_event_recv(&ControlEvent, controlEventEaix4, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_NO, &e) == RT_EOK)
        {
            if (e & controlEventEaix4)
            {
                for (i = 1; i < 5; i++)
                {
                    for (rt_uint16_t x = 0; x < around[i].number; x++)
                    {
                        if (around[i].ap[x].distance <= 800)
                        {
//                      rt_kprintf("warning circle:%d angle:%d distance:%d\n", i, ((rt_uint32_t)around[i].ap[x].angle) * 100 / 64, ((rt_uint32_t)around[i].ap[x].distance) / 4);
                            buf = (rt_uint32_t)around[i].ap[x].angle * 100 / 64 ;
                            if (buf >= 31500 || buf < 4500)
                            {
                                if (buf >= 31500)
                                {
                                    fsignalL++;
                                }
                                else if (buf < 4500)
                                {
                                    fsignalR++;
                                }

                            }
                            else if (buf >= 4500 && buf < 13500)
                            {
                                if (buf >= 4500)
                                {
                                    rsignalL++;
                                }
                                else if (buf < 13500)
                                {
                                    rsignalR++;
                                }
                            }
                            else if (buf >= 13500 && buf < 22500)
                            {
                                if (buf >= 13500)
                                {
                                    bsignalL++;
                                }
                                else if (buf < 22500)
                                {
                                    bsignalR++;
                                }
                            }
                            else if (buf >= 22500 && buf < 31500)
                            {
                                if (buf >= 22500)
                                {
                                    lsignalL++;
                                }
                                else if (buf < 31500)
                                {
                                    lsignalR++;
                                }
                            }
                        }
                    }
                }
                if ((fsignalL == 0) && (fsignalR == 0))
                {
                    carForward();
                }
                else if (fsignalL > fsignalR)
                {
                    carRight();
                }
                else if (fsignalL <= fsignalR)
                {
                    carLeft();
                }
                fsignalL = 0;
                fsignalR = 0;
                bsignalL = 0;
                bsignalR = 0;
                lsignalL = 0;
                lsignalR = 0;
                rsignalL = 0;
                rsignalR = 0;
                Eaix4Scaning();
            }
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
                           1024,
                           3,
                           10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
INIT_APP_EXPORT(master_init);
