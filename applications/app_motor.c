#include <rthw.h>
#include <rtthread.h>
#include "drv_l298n.h"
#include "app_motor.h"
#include <finsh.h>

#define forward "-f"
#define back    "-b"
#define left    "-l"
#define right   "-r"
#define stop    "-s"

void carForward()
{
			rt_pin_write(motor_in_1, PIN_LOW);
			rt_pin_write(motor_in_2, PIN_HIGH);
			rt_pin_write(motor_in_3, PIN_LOW);
			rt_pin_write(motor_in_4, PIN_HIGH);	
}

void carBack()
{
			rt_pin_write(motor_in_1, PIN_HIGH);
			rt_pin_write(motor_in_2, PIN_LOW);
			rt_pin_write(motor_in_3, PIN_HIGH);
			rt_pin_write(motor_in_4, PIN_LOW);	
}

void carLeft()
{
			rt_pin_write(motor_in_1, PIN_LOW);
			rt_pin_write(motor_in_2, PIN_HIGH);
			rt_pin_write(motor_in_3, PIN_HIGH);
			rt_pin_write(motor_in_4, PIN_LOW);	
}
void carMove(int argc, char** argv)
{
	rt_kprintf("%s is runed!\n", argv[0]);
	if (argc > 1)
	{
		rt_kprintf("argv[1]: %s\n", argv[1]);
		if(!rt_strcmp(argv[1],forward))
		{
			rt_kprintf("ok1\n");			
			rt_pin_write(motor_in_1, PIN_LOW);
			rt_pin_write(motor_in_2, PIN_HIGH);
			rt_pin_write(motor_in_3, PIN_LOW);
			rt_pin_write(motor_in_4, PIN_HIGH);
		}
		else if(!rt_strcmp(argv[1],back))
		{
			rt_kprintf("ok2\n");
			rt_pin_write(motor_in_1, PIN_HIGH);
			rt_pin_write(motor_in_2, PIN_LOW);
			rt_pin_write(motor_in_3, PIN_HIGH);
			rt_pin_write(motor_in_4, PIN_LOW);
		}
		else if(!rt_strcmp(argv[1],left))
		{
			rt_kprintf("ok2\n");
			rt_pin_write(motor_in_1, PIN_LOW);
			rt_pin_write(motor_in_2, PIN_HIGH);
			rt_pin_write(motor_in_3, PIN_HIGH);
			rt_pin_write(motor_in_4, PIN_LOW);
		}
		else if(!rt_strcmp(argv[1],right))
		{
			rt_kprintf("ok2\n");
			
			rt_pin_write(motor_in_1, PIN_HIGH);
			rt_pin_write(motor_in_2, PIN_LOW);		
			rt_pin_write(motor_in_3, PIN_LOW);
			rt_pin_write(motor_in_4, PIN_HIGH);
		}
		else if(!rt_strcmp(argv[1],stop))
		{
			rt_kprintf("ok2\n");
			
			rt_pin_write(motor_in_1, PIN_HIGH);
			rt_pin_write(motor_in_2, PIN_HIGH);		
			rt_pin_write(motor_in_3, PIN_HIGH);
			rt_pin_write(motor_in_4, PIN_HIGH);
		}
	}
}
MSH_CMD_EXPORT(carMove, control car movement (-f -b -l -r -s) );
