#include <rthw.h>
#include <rtthread.h>

#include "app_wirelessuart.h"

/* 串口接收事件标志 */
#define WIRELESS_RX_EVENT (1 << 0)

/* 事件控制块 */
static struct rt_event wirelessevent;
/* 串口设备句柄 */
static rt_device_t wireless_device = RT_NULL;

/* 回调函数 */
static rt_err_t wireless_intput(rt_device_t dev, rt_size_t size)
{
    /* 发送事件 */
    rt_event_send(&wirelessevent, WIRELESS_RX_EVENT);

    return RT_EOK;
}

rt_uint8_t wireless_getchar(void)
{
    rt_uint32_t e;
    rt_uint8_t ch;

    /* 读取1字节数据 */
    while (rt_device_read(wireless_device, 0, &ch, 1) != 1)
    {
        /* 接收事件 */
        rt_event_recv(&wirelessevent, WIRELESS_RX_EVENT, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e);
    }

    return ch;
}
void wireless_putchar(const rt_uint8_t c)
{
    rt_size_t len = 0;
    rt_uint32_t timeout = 0;
    do
    {
        len = rt_device_write(wireless_device, 0, &c, 1);
        timeout++;
    }
    while (len != 1 && timeout < 500);
}

void wireless_putstring(const rt_uint8_t *s)
{
    while (*s)
    {
        wireless_putchar(*s++);
    }
}

rt_err_t wireless_open(const char *name)
{
    rt_err_t res;

    /* 查找系统中的串口设备 */
    wireless_device = rt_device_find(name);
    /* 查找到设备后将其打开 */
    if (wireless_device != RT_NULL)
    {

        res = rt_device_set_rx_indicate(wireless_device, wireless_intput);
        /* 检查返回值 */
        if (res != RT_EOK)
        {
            rt_kprintf("set %s rx indicate error.%d\n", name, res);
            return -RT_ERROR;
        }

        /* 打开设备，以可读写、中断方式 */
        res = rt_device_open(wireless_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
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

    /* 初始化事件对象 */
    rt_event_init(&wirelessevent, "wireless", RT_IPC_FLAG_FIFO);

    return RT_EOK;
}

void wireless_thread_entry(void *parameter)
{
    /* 打开串口 */
    if (wireless_open("uart6") != RT_EOK)
    {
        rt_kprintf("uart6 open error.\n");
    }
    while (1)
    {
			rt_thread_delay(rt_tick_from_millisecond(20));
//        EAI_X4_GetDate();
    }
}

void wireless_init(void)
{
    rt_thread_t tid;

    /* 创建test线程 *////////////////////////////////////////
    tid = rt_thread_create("wireless",
                           wireless_thread_entry,
                           RT_NULL,
                           256,
                           3,
                           10);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);	
}
