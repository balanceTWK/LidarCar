#include "master_thread.h"
#include "app_eaix4.h"
#include "app_wirelessuart.h"
#include "app_mpu9250.h"
#include "app_beep.h"
#include "EAI_X4.h"
#include "app_move.h"
#include "app_speed.h"

struct rt_event ControlEvent;
struct rt_mutex SpeedMutex;
struct rt_mutex Mpu9250Mutex;

rt_int16_t wantspeed1 = 0;
rt_int16_t wantspeed2 = 0;

volatile float mypitch, myroll, myyaw;

void master_thread_entry(void *parameter)
{
    rt_uint32_t e;
    rt_uint8_t i;
    rt_uint32_t buf;
    rt_int32_t fsignal = 0;
    rt_int32_t ffsignal = 0;
    rt_int32_t swing;

    while (1)
    {
        /* 接收事件 */
        if (rt_event_recv(&ControlEvent, controlEventEaix4, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e) == RT_EOK)
        {
            if (e & controlEventEaix4)
            {
                for (i = 1; i < 5; i++)
                {
                    for (rt_uint16_t x = 0; x < around[i].number; x++)
                    {
                        if (around[i].ap[x].distance <= 1200)
                        {
                            if (around[i].ap[x].distance <= 600)
                            {
                                buf = (rt_uint32_t)around[i].ap[x].angle * 100 / 64 ;
                                if ((buf > 32500) || (buf < 3500))
                                {
                                    if (buf > 32500)
                                    {
                                        fsignal++;
                                    }
                                    else if (buf < 3500)
                                    {
                                        fsignal--;
                                    }
                                }
                            }
                            else
                            {
                                buf = (rt_uint32_t)around[i].ap[x].angle * 100 / 64 ;
                                if (buf > 18000)
                                {
                                    ffsignal++;
                                }
                                else if (buf < 18000)
                                {
                                    ffsignal--;
                                }
                            }
                        }
                    }
                }
                if (fsignal != 0)
                {
                    if ((fsignal >= -10) && (fsignal <= 10))
                    {
                        carStop();
                        swing++;
                    }
                    else if (fsignal > 10)
                    {
                        carRight();
                        swing++;
                    }
                    else
                    {
                        carLeft();
                        swing++;
                    }

                    if (swing >= 4)
                    {
                        swing = 0;
                        ffsignal += fsignal;
                        if (ffsignal > 0)
                        {
                            carRight();
                            beep(1);
                            rt_thread_delay(rt_tick_from_millisecond(2000));
                            beep(0);
                        }
                        else
                        {
                            carLeft();
                            beep(1);
                            rt_thread_delay(rt_tick_from_millisecond(2000));
                            beep(0);
                        }
                    }
                }
                else
                {
                    swing = 0;
                    carForward();
                }
                fsignal = 0;
                ffsignal = 0;
                Eaix4Scaning();
            }
        }
        else
        {
            rt_thread_delay(rt_tick_from_millisecond(100));
        }
    }
}

int master_init()
{
    rt_thread_t tid;

    rt_kprintf("master_init.\n");

    rt_event_init(&ControlEvent, "Control", RT_IPC_FLAG_FIFO);
    rt_mutex_init(&SpeedMutex, "speed", RT_IPC_FLAG_FIFO);
    rt_mutex_init(&Mpu9250Mutex, "mpu9250", RT_IPC_FLAG_FIFO);

    eaix4_init();
    wireless_init();
    mpu9250_init();
    speed_init();
    rt_kprintf("master_done.\n");
    /* 创建test线程 */
    tid = rt_thread_create("master",
                           master_thread_entry,
                           RT_NULL,
                           4096,
                           3,
                           20);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
INIT_APP_EXPORT(master_init);
