#include <rthw.h>
#include <rtthread.h>

#include "app_slam.h"

rt_uint8_t Eaix4Buff[45000];

static struct rt_messagequeue  	slam_uart_mq;

/* 串口设备句柄 */
static rt_device_t slam_uart_device = RT_NULL;

/* 回调函数 */
static rt_err_t slam_uart_intput(rt_device_t dev, rt_size_t size)
{
    rt_uint8_t ch;
    /* 读取1字节数据 */
    if(rt_device_read(slam_uart_device, 0, &ch, 1) == 1)
    {
        rt_mq_send (&slam_uart_mq, &ch, 1);
    }
    return RT_EOK;
}

rt_uint8_t slam_uart_getchar(void)
{
    rt_uint8_t ch;
		rt_mq_recv(&slam_uart_mq,&ch,1,RT_WAITING_FOREVER);
    return ch;
}
void slam_uart_putchar(const rt_uint8_t c)
{
    rt_size_t len = 0;
    rt_uint32_t timeout = 0;
    do
    {
        len = rt_device_write(slam_uart_device, 0, &c, 1);
        timeout++;
    }
    while (len != 1 && timeout < 500);
}

void slam_uart_putstring(const rt_uint8_t *s)
{
    while (*s)
    {
        slam_uart_putchar(*s++);
    }
}

rt_err_t slam_uart_open(const char *name)
{
    rt_err_t res;

    /* 查找系统中的串口设备 */
    slam_uart_device = rt_device_find(name);
    /* 查找到设备后将其打开 */
    if (slam_uart_device != RT_NULL)
    {

        res = rt_device_set_rx_indicate(slam_uart_device, slam_uart_intput);
        /* 检查返回值 */
        if (res != RT_EOK)
        {
            rt_kprintf("set %s rx indicate error.%d\n", name, res);
            return -RT_ERROR;
        }

        /* 打开设备，以可读写、中断方式 */
        res = rt_device_open(slam_uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
        /* 检查返回值 */
        if (res != RT_EOK)
        {
            rt_kprintf("open %s device error.%d\n", name, res);
            return -RT_ERROR;
        }

    }
    else
    {
        rt_kprintf("can't find %s device.\n", name);
        return -RT_ERROR;
    }

    /* 初始化消息队列对象 */	
		rt_mq_init(&slam_uart_mq, "EAIX4", Eaix4Buff, 1,sizeof(Eaix4Buff),RT_IPC_FLAG_FIFO);

    return RT_EOK;
}



