#include <rthw.h>
#include <rtthread.h>
#include "app_command.h"
#include <finsh.h>

#include "app_slam.h"

#define stop                    "-s"
#define force_stoppage          "-fs"
#define start_scanning          "-sc"
#define force_start_scanning    "-fsc"
#define get_version_information "-gvf"
#define get_health_status       "-ghs"

/* ÊÂ¼þ¿ØÖÆ¿é */
struct rt_event ControlEvent;;

void Eaix4Scaning()
{
    slam_uart_putchar(0xA5);
    slam_uart_putchar(0x60);
}

void Eaix4stop()
{
    slam_uart_putchar(0xA5);
    slam_uart_putchar(0x65);
}
void Eaix4Version()
{
    slam_uart_putchar(0xA5);
    slam_uart_putchar(0x90);
}

void Eaix4Health()
{
    slam_uart_putchar(0xA5);
    slam_uart_putchar(0x91);
}

void command(int argc, char **argv)
{
    if (argc > 1)
    {
        if (!rt_strcmp(argv[1], stop))
        {
            Eaix4stop();
        }
        else if (!rt_strcmp(argv[1], force_stoppage))
        {
            slam_uart_putchar(0xA5);
            slam_uart_putchar(0x00);
        }
        else if (!rt_strcmp(argv[1], start_scanning))
        {
            Eaix4Scaning();
//          rt_thread_delay(rt_tick_from_millisecond(150));
        }
        else if (!rt_strcmp(argv[1], force_start_scanning))
        {
            slam_uart_putchar(0xA5);
            slam_uart_putchar(0x61);
        }
        else if (!rt_strcmp(argv[1], get_version_information))
        {
            Eaix4Version();
        }
        else if (!rt_strcmp(argv[1], get_health_status))
        {
            Eaix4Health();
        }
    }
}
MSH_CMD_EXPORT(command, -s - fs - sc - fsc - gvf - ghs);
