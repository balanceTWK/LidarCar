#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <rthw.h>
#include <rtthread.h>
/* 串口接收事件标志 */
#define controlEventEaix4 (1 << 0)

void Eaix4Scaning(void);
void Eaix4stop(void);
void Eaix4Version(void);
void Eaix4Health(void);
/* 事件控制块 */
extern struct rt_event ControlEvent;;
#endif
