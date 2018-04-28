#include <rthw.h>
#include <rtthread.h>

#include "drv_l298n.h"
#include "app_move.h"
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

void carRight()
{
			rt_pin_write(motor_in_1, PIN_HIGH);
			rt_pin_write(motor_in_2, PIN_LOW);		
			rt_pin_write(motor_in_3, PIN_LOW);
			rt_pin_write(motor_in_4, PIN_HIGH);	
}

void carStop()
{
			rt_pin_write(motor_in_1, PIN_HIGH);
			rt_pin_write(motor_in_2, PIN_HIGH);		
			rt_pin_write(motor_in_3, PIN_HIGH);
			rt_pin_write(motor_in_4, PIN_HIGH);	
}

void carMove(int argc, char** argv)
{
	rt_kprintf("%s is runed!\n", argv[0]);
	if (argc > 1)
	{
		rt_kprintf("argv[1]: %s\n", argv[1]);
		if(!rt_strcmp(argv[1],forward))
		{
			rt_kprintf("forward\n");			
			carForward();
		}
		else if(!rt_strcmp(argv[1],back))
		{
			rt_kprintf("back\n");
			carBack();
		}
		else if(!rt_strcmp(argv[1],left))
		{
			rt_kprintf("left\n");			
			carLeft();
		}
		else if(!rt_strcmp(argv[1],right))
		{
			rt_kprintf("right\n");			
			carRight();
		}
		else if(!rt_strcmp(argv[1],stop))
		{
			rt_kprintf("stop\n");
			carStop();
		}
	}
}
MSH_CMD_EXPORT(carMove, control car movement (-f -b -l -r -s) );
