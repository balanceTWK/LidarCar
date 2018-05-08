#include <rthw.h>
#include <rtthread.h>
#include "app_command.h"
#include <finsh.h>
#include "master_thread.h"
#include "app_eaix4.h"

#define rangeFinder             "-d"
#define Refresh          				"-f"

void command(int argc, char **argv)
{
    if (argc > 1)
    {
        if (!rt_strcmp(argv[1], rangeFinder)) //测距模式
        {
					Eaix4Scaning();
					rt_mutex_take(&StatusMutex, RT_WAITING_FOREVER);
					CarStatus&=0xFFFFFFF7;
					rt_mutex_release(&StatusMutex);
        }
        else if (!rt_strcmp(argv[1], Refresh)) //刷新一次地图
        {
					Eaix4Scaning();
					rt_mutex_take(&StatusMutex, RT_WAITING_FOREVER);
					CarStatus|=0x00000008;
					rt_mutex_release(&StatusMutex);
        }
    }
}
MSH_CMD_EXPORT(command,( -d : distance mode) ( -f :  Refresh map) );
