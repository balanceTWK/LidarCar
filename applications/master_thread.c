#include "master_thread.h"
#include "app_eaix4.h"
#include "app_wirelessuart.h"
#include "app_mpu9250.h"
#include "app_beep.h"
#include "EAI_X4.h"
#include "app_move.h"
#include "app_speed.h"
#include "arm_math.h"
//#include "math.h"

struct rt_event ControlEvent;
struct rt_mutex SpeedMutex;
struct rt_mutex Mpu9250Mutex;

rt_int16_t wantspeed1 = 0;
rt_int16_t wantspeed2 = 0;

volatile float mypitch, myroll, myyaw;

rt_uint8_t map[200][200];

#define ORIGINX 100
#define ORIGINY 100

#define ANGLE(x) (float32_t)((x)*2*PI/23040)

void master_thread_entry(void *parameter)
{
    rt_uint32_t e;
    rt_uint8_t i;
    float32_t sinx, cosx;
    rt_int32_t lx, ly;

    while (1)
    {
        /* 接收事件 */
        if (rt_event_recv(&ControlEvent, controlEventEaix4, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e) == RT_EOK)
        {
            if (e & controlEventEaix4)
            {
                for (rt_uint16_t x = 0; x < 200; x++)
                {
                    for (rt_uint16_t y = 0; y < 200; y++)
                    {
											if(map[x][y]>50)
											{
												printf(" >50 map[%d][%d]: value:%d \r\n",x-100,y-100,map[x][y]);
											}
											map[x][y] = 0;
                    }
                }
                for (i = 1; i < 5; i++)
                {
                    for (rt_uint16_t x = 0; x < around[i].number; x++)
                    {
                        sinx = arm_sin_f32((float32_t)ANGLE(around[i].ap[x].angle));
                        cosx = arm_cos_f32((float32_t)ANGLE(around[i].ap[x].angle));
                        lx = sinx * around[i].ap[x].distance;
                        ly = cosx * around[i].ap[x].distance;
                        lx = lx / 400;
                        ly = ly / 400;
                        if ((lx >= -100) && (lx <= 100) && (ly >= -100) && (ly <= 100))
                        {
                            map[ORIGINX + lx][ORIGINY + ly]++;
                        }
                    }
                }
                beep(1);
                rt_thread_delay(rt_tick_from_millisecond(50));
                beep(0);
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
