#include "drv_l298n.h"

#include "board.h"
#include <rtdevice.h>
#include <rthw.h>
//#include <rtthread.h>





static void stm32f429_l298n_pin_init()
{

	rt_pin_mode(motor_in_1, PIN_MODE_OUTPUT);
	rt_pin_mode(motor_in_2, PIN_MODE_OUTPUT);
	rt_pin_mode(motor_in_3, PIN_MODE_OUTPUT);
	rt_pin_mode(motor_in_4, PIN_MODE_OUTPUT);
	rt_pin_write(motor_in_1, PIN_HIGH);
	rt_pin_write(motor_in_2, PIN_HIGH);
	rt_pin_write(motor_in_3, PIN_HIGH);
	rt_pin_write(motor_in_4, PIN_HIGH);
	
}

int hw_l298n_init(void)
{
	stm32f429_l298n_pin_init();

	return RT_EOK;
}
INIT_BOARD_EXPORT(hw_l298n_init);
/* end of i2c driver */
