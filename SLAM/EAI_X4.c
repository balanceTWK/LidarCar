#include "EAI_X4.h"
#include <drivers/pin.h>
#include "app_eaix4.h"
#include "app_beep.h"
#include "master_thread.h"

#define PH 0x55AA
#define StartCTLSN 0x0101

Around around[5];
rt_int8_t circle = -1;
rt_int16_t count = 0;
//rt_int16_t number[5];
void manipulationData()
{
    PackAge package;
    rt_uint8_t ct;

    rt_uint8_t i;
    rt_uint16_t angle;
    rt_uint16_t angleDiff;
    while (1)
    {
        if (eaix4_getchar() == 0xAA)
        {
            if (eaix4_getchar() == 0x55)
            {
                ct = eaix4_getchar();
                if (ct == 0x00)
                {
                    package.lsn = eaix4_getchar();
                    package.fsa = eaix4_getchar();
                    package.fsa = eaix4_getchar() << 8;
                    package.lsa = eaix4_getchar();
                    package.lsa = eaix4_getchar() << 8;
                    package.cs = eaix4_getchar();
                    package.cs = eaix4_getchar() << 8;
                    for (i = 0; i < package.lsn; i++)
                    {
                        package.buf[i] = eaix4_getchar();
                        package.buf[i] = eaix4_getchar() << 8;
                    }
                    angleDiff = ((package.lsa >> 1) - (package.fsa >> 1)) / package.lsn;
                    for (i = 0; i < package.lsn; i++)
                    {
                        angle = i * angleDiff;
                        if (package.buf[i] != 0)
                        {
                            around[circle - 1].ap[count].angle = (package.fsa >> 1) + angle;
                            around[circle - 1].ap[count].distance = package.buf[i];
                            count++;
                        }
                    }
                    angle = 0;
                }
                else if (ct == 0x01)//起始点
                {
                    circle++;
                    if (circle > 1)
                    {
                        around[circle - 2].number = count;
                        count = 0;
                        if (circle == 6)
                        {
                            circle = 0;
                            eaix4_putchar(0xA5);//停止扫描
                            eaix4_putchar(0x65);
                            /* 发送事件 */
                            rt_event_send(&ControlEvent, controlEventEaix4);
                            break;
                        }
                    }
                    package.lsn = eaix4_getchar();
                    package.fsa = eaix4_getchar();
                    package.fsa = eaix4_getchar() << 8;
                    package.lsa = eaix4_getchar();
                    package.lsa = eaix4_getchar() << 8;
                    package.cs = eaix4_getchar();
                    package.cs = eaix4_getchar() << 8;
                    package.buf[0] = eaix4_getchar();
                    package.buf[0] = eaix4_getchar() << 8;
                    around[circle - 1].firstpoint.angle = (package.fsa >> 1);
                    around[circle - 1].firstpoint.distance = package.buf[0];
                }
            }
        }
    }
}

void EAI_X4_GetDate()
{
    rt_uint8_t reply[5];
    rt_uint8_t Eaix4Version[20];
    rt_uint8_t Eaix4Health[3];
    rt_uint8_t i;
    while (1)
    {
        while (1)
        {
            if (eaix4_getchar() == 0xA5)
            {
                if (eaix4_getchar() == 0x5A)
                {
                    for (i = 0; i < 5; i++)
                    {
                        reply[i] = eaix4_getchar();
                    }
                    if ((reply[0] == 0x05) && (reply[1] == 0x00) && (reply[2] == 0x00) && (reply[3] == 0x40) && (reply[4] == 0x81)) //接收扫描数据
                    {
                        rt_kprintf("\nStart receiving point data.\n");
                        manipulationData();
                    }
                    else if ((reply[0] == 0x14) && (reply[1] == 0x00) && (reply[2] == 0x00) && (reply[3] == 0x00) && (reply[4] == 0x04)) //接收设备信息
                    {
                        for (i = 0; i < 20; i++)
                        {
                            Eaix4Version[i] = eaix4_getchar();
                        }
                        rt_kprintf("\nmodel:             %d\n", Eaix4Version[0]);
                        rt_kprintf("Firmware version : %d.%d\n", Eaix4Version[2], Eaix4Version[1]);
                        rt_kprintf("Hardware version:  %d.\n", Eaix4Version[3]);
                        rt_kprintf("serials number:    %d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d\n", Eaix4Version[4], Eaix4Version[5], Eaix4Version[6], Eaix4Version[7],
                                   Eaix4Version[8], Eaix4Version[9], Eaix4Version[10], Eaix4Version[11],
                                   Eaix4Version[12], Eaix4Version[13], Eaix4Version[14], Eaix4Version[15],
                                   Eaix4Version[16], Eaix4Version[17], Eaix4Version[18], Eaix4Version[19]);
                    }
                    else if ((reply[0] == 0x03) && (reply[1] == 0x00) && (reply[2] == 0x00) && (reply[3] == 0x00) && (reply[4] == 0x06)) //接收健康状态
                    {
                        for (i = 0; i < 3; i++)
                        {
                            Eaix4Health[i] = eaix4_getchar();
                        }
                        if (Eaix4Health[0] == 0x00)
                        {
                            rt_kprintf("\nEquipment is normal.\n");
                        }
                        else
                        {
                            rt_kprintf("\nError code %d.%d.%d\n", Eaix4Health[0], Eaix4Health[1], Eaix4Health[2]);
                        }
                    }
                }
            }
        }
    }
}

