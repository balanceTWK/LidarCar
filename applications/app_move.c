#include <rthw.h>
#include <rtthread.h>

#include "drv_l298n.h"
#include "app_move.h"
#include <finsh.h>

#include "master_thread.h"

#define forward "-f"
#define back    "-b"
#define left    "-l"
#define right   "-r"
#define stop    "-s"

void carForward()
{
    wantspeed1 = 80;
    wantspeed2 = 80;
    rt_mutex_release(&SpeedMutex);
}

void carBack()
{
    wantspeed1 = -80;
    wantspeed2 = -80;
    rt_mutex_release(&SpeedMutex);
}

void carLeft()
{
    wantspeed1 = -80;
    wantspeed2 = 80;
    rt_mutex_release(&SpeedMutex);
}

void carRight()
{
    wantspeed1 = 80;
    wantspeed2 = -80;
    rt_mutex_release(&SpeedMutex);
}

void carStop()
{
    wantspeed1 = 0;
    wantspeed2 = 0;
    rt_mutex_release(&SpeedMutex);
}

void carMove(int argc, char **argv)
{
    rt_kprintf("%s is runed!\n", argv[0]);
    if (argc > 1)
    {
        rt_kprintf("argv[1]: %s\n", argv[1]);
        if (!rt_strcmp(argv[1], forward))
        {
            rt_kprintf("forward\n");
            carForward();
        }
        else if (!rt_strcmp(argv[1], back))
        {
            rt_kprintf("back\n");
            carBack();
        }
        else if (!rt_strcmp(argv[1], left))
        {
            rt_kprintf("left\n");
            carLeft();
        }
        else if (!rt_strcmp(argv[1], right))
        {
            rt_kprintf("right\n");
            carRight();
        }
        else if (!rt_strcmp(argv[1], stop))
        {
            rt_kprintf("stop\n");
            carStop();
        }
    }
}
MSH_CMD_EXPORT(carMove, control car movement(-f - b - l - r - s));
