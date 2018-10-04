#ifndef __SLAMRADAR_H__
#define __SLAMRADAR_H__

#include <rtthread.h>
#include <rthw.h>



//static struct rt_event package_event;


rt_err_t slam_uart_open(const char *name);
rt_uint8_t slam_uart_getchar(void);
void slam_uart_putchar(const rt_uint8_t c);
void slam_uart_putstring(const rt_uint8_t *s);

#endif
