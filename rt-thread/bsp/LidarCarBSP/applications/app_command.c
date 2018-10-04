#include <rthw.h>
#include <rtthread.h>
#include "app_command.h"
#include <finsh.h>
#include "master_thread.h"
#include "app_eaix4.h"
#include "app_move.h"

#define rangeFinder             "-d"
#define Refresh          				"-f"
#define TEST          				"-t"
extern volatile float savemypitch, savemyroll, savemyyaw;
void command(int argc, char **argv)
{
    if (argc > 1)
    {
        if (!rt_strcmp(argv[1], rangeFinder)) //测距模式
        {
					while(1)
					{
							rt_mutex_take(&Mpu9250Mutex, RT_WAITING_FOREVER);
							savemyyaw = myyaw;
							rt_mutex_release(&Mpu9250Mutex);
							printf("myyaw:%f \r\n",savemyyaw);
							rt_thread_delay(rt_tick_from_millisecond(20));			
					}
        }
        else if (!rt_strcmp(argv[1], Refresh)) //刷新一次地图
        {
					Eaix4Scaning();
					rt_mutex_take(&StatusMutex, RT_WAITING_FOREVER);
					CarStatus|=0x00000008;
					rt_mutex_release(&StatusMutex);
        }
        else if (!rt_strcmp(argv[1], TEST)) //test
        {
					rt_event_send(&ControlEvent, control1);
        }
        else
        {
          rt_kprintf("ERROR command !\n");
        }
    }
}
MSH_CMD_EXPORT(command,( -d : distance mode) ( -f :  Refresh map) );
