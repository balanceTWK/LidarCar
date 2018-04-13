#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#include "drv_pwm.h"

#include "stm32f4xx_hal.h"

/*user should change this to adapt specific board*/
#define pwm1_PIN                 GPIO_PIN_5
#define pwm1_PORT                GPIOE
#define pwm1__PORT_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()

#define pwm2_PIN                 GPIO_PIN_6
#define pwm2_PORT                GPIOE
#define pwm2__PORT_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE()

#define radar_pwm_PIN                 GPIO_PIN_15
#define radar_pwm_PORT                GPIOA
#define radar_pwm__PORT_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()


TIM_HandleTypeDef TIMx_Handler;         //定时器3PWM句柄 
TIM_OC_InitTypeDef TIMx_CHxHandler;	    //定时器3通道4句柄
//TIM9 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM_2_9_PWM_Init(rt_uint16_t arr,rt_uint16_t psc)
{ 
    TIMx_Handler.Instance=TIM9;            //定时器3
    TIMx_Handler.Init.Prescaler=psc;       //定时器分频
    TIMx_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIMx_Handler.Init.Period=arr;          //自动重装载值
    TIMx_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIMx_Handler);       //初始化PWM    
    TIMx_CHxHandler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIMx_CHxHandler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIMx_CHxHandler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIMx_Handler,&TIMx_CHxHandler,TIM_CHANNEL_1);//配置TIM3通道4
		HAL_TIM_PWM_ConfigChannel(&TIMx_Handler,&TIMx_CHxHandler,TIM_CHANNEL_2);//配置TIM3通道4
		HAL_TIM_PWM_Start(&TIMx_Handler,TIM_CHANNEL_1);//开启PWM通道4
		HAL_TIM_PWM_Start(&TIMx_Handler,TIM_CHANNEL_2);//开启PWM通道4
		
    TIMx_Handler.Instance=TIM2;            //定时器3
    HAL_TIM_PWM_Init(&TIMx_Handler);       //初始化PWM
    HAL_TIM_PWM_ConfigChannel(&TIMx_Handler,&TIMx_CHxHandler,TIM_CHANNEL_1);//配置TIM3通道4
    HAL_TIM_PWM_Start(&TIMx_Handler,TIM_CHANNEL_1);//开启PWM通道4	
}


//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_TIM9_CLK_ENABLE();			//使能定时器9
    pwm1__PORT_CLK_ENABLE();			//开启GPIOE时钟
	  pwm2__PORT_CLK_ENABLE();			//开启GPIOE时钟

	
    GPIO_Initure.Pin=pwm1_PIN|pwm2_PIN;           	//PB1
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
		GPIO_Initure.Alternate= GPIO_AF3_TIM9;	//复用为TIM9_CH1`2
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
	
		__HAL_RCC_TIM2_CLK_ENABLE();			//使能定时器2
    radar_pwm__PORT_CLK_ENABLE();			//开启GPIOA时钟
		GPIO_Initure.Pin=radar_pwm_PIN;           	//PB1
		GPIO_Initure.Pull=GPIO_PULLUP;          //下拉
		GPIO_Initure.Alternate= GPIO_AF1_TIM2;	//复用为TIM9_CH1`2
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//设置TIM9通道1 2的占空比
//compare:比较值
void TIM_SetTIM2Compare(rt_uint32_t compare)
{
	TIM2->CCR1=compare; 
}


//设置TIM9通道1 2的占空比
//compare:比较值
void TIM_SetTIM9Compare(rt_uint32_t compare1,rt_uint32_t compare2)
{
	TIM9->CCR1=compare1; 
	TIM9->CCR2=compare2;
}


	

int hw_PWM_init(void)
{
	TIM_2_9_PWM_Init(2000-1,9-1);      //180M/9=20M的计数频率，自动重装载为2000，那么PWM频率为20M/2000=10kHZ

  TIM_SetTIM9Compare(300,300);
	TIM_SetTIM2Compare(300);
	return RT_EOK;
}
INIT_BOARD_EXPORT(hw_PWM_init);
/* end of i2c driver */

