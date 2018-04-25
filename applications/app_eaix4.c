#include <rthw.h>
#include <rtthread.h>

#include "app_eaix4.h"

rt_uint8_t Eaix4Buff[45000];

static struct rt_messagequeue  	Eaix4Mq;

/* 串口设备句柄 */
static rt_device_t eaix4_device = RT_NULL;

/* 回调函数 */
static rt_err_t eaix4_intput(rt_device_t dev, rt_size_t size)
{
    rt_uint8_t ch;
    /* 读取1字节数据 */
    if(rt_device_read(eaix4_device, 0, &ch, 1) == 1)
    {
        rt_mq_send (&Eaix4Mq, &ch, 1);
    }
    return RT_EOK;
}

rt_uint8_t  eaix4_getchar(void)
{
    rt_uint8_t ch;
		rt_mq_recv(&Eaix4Mq,&ch,1,RT_WAITING_FOREVER);
    return ch;
}

void eaix4_putchar(const rt_uint8_t c)
{
    rt_size_t len = 0;
    rt_uint32_t timeout = 0;
    do
    {
        len = rt_device_write(eaix4_device, 0, &c, 1);
        timeout++;
    }
    while (len != 1 && timeout < 500);
}

void eaix4_putstring(const rt_uint8_t *s)
{
    while (*s)
    {
        eaix4_putchar(*s++);
    }
}

rt_err_t eaix4_open(const char *name)
{
    rt_err_t res;

    /* 查找系统中的串口设备 */
    eaix4_device = rt_device_find(name);
    /* 查找到设备后将其打开 */
    if (eaix4_device != RT_NULL)
    {

        res = rt_device_set_rx_indicate(eaix4_device, eaix4_intput);
        /* 检查返回值 */
        if (res != RT_EOK)
        {
            rt_kprintf("set %s rx indicate error.%d\n", name, res);
            return -RT_ERROR;
        }

        /* 打开设备，以可读写、中断方式 */
        res = rt_device_open(eaix4_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
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
		rt_mq_init(&Eaix4Mq, "EAIX4", Eaix4Buff, 1,sizeof(Eaix4Buff),RT_IPC_FLAG_FIFO);

    return RT_EOK;
}

#include <finsh.h>
#define stop                    "-s"
#define force_stoppage          "-fs"
#define start_scanning          "-sc"
#define force_start_scanning    "-fsc"
#define get_version_information "-gvf"
#define get_health_status       "-ghs"

void Eaix4Scaning()
{
    eaix4_putchar(0xA5);
    eaix4_putchar(0x60);
}
void Eaix4stop()
{
    eaix4_putchar(0xA5);
    eaix4_putchar(0x65);
}
void Eaix4Version()
{
    eaix4_putchar(0xA5);
    eaix4_putchar(0x90);
}
void Eaix4Health()
{
    eaix4_putchar(0xA5);
    eaix4_putchar(0x91);
}

void eaix4cmd(int argc, char **argv)
{
    if (argc > 1)
    {
        if (!rt_strcmp(argv[1], stop))
        {
            Eaix4stop();
        }
        else if (!rt_strcmp(argv[1], force_stoppage))
        {
            eaix4_putchar(0xA5);
            eaix4_putchar(0x00);
        }
        else if (!rt_strcmp(argv[1], start_scanning))
        {
            Eaix4Scaning();
        }
        else if (!rt_strcmp(argv[1], force_start_scanning))
        {
            eaix4_putchar(0xA5);
            eaix4_putchar(0x61);
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
MSH_CMD_EXPORT(eaix4cmd, -s - fs - sc - fsc - gvf - ghs);


#include "EAI_X4.h"
void eaix4_thread_entry(void *parameter)
{
    /* 打开串口 */
    if (eaix4_open("uart2") != RT_EOK)
    {
        rt_kprintf("uart2 open error.\n");
    }
    while (1)
    {
			EAI_X4_GetDate();
    }
}

void eaix4_init(void)
{
    rt_thread_t tid;

    /* 创建test线程 *////////////////////////////////////////
    tid = rt_thread_create("eaix4",
                           eaix4_thread_entry,
                           RT_NULL,
                           384,
                           3,
                           30);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);	
		
}
