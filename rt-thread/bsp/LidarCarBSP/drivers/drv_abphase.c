#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#include "drv_abphase.h"

#include "stm32f4xx_hal.h"

TIM_HandleTypeDef TIM3_Handler;         //定时器3PWM句柄

TIM_HandleTypeDef TIM8_Handler;         //定时器8PWM句柄

void TIM_abphase_Init()
{
    TIM_Encoder_InitTypeDef sConfig;
    TIM_MasterConfigTypeDef sMasterConfig;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    TIM3_Handler.Instance = TIM3;
    TIM3_Handler.Init.Prescaler = 0;
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM3_Handler.Init.Period = 0xffff;
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 5;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 5;

    HAL_TIM_Encoder_Init(&TIM3_Handler, &sConfig);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    HAL_TIMEx_MasterConfigSynchronization(&TIM3_Handler, &sMasterConfig);
    HAL_TIM_Encoder_Start(&TIM3_Handler, TIM_CHANNEL_ALL);

    TIM8_Handler.Instance = TIM8;
    TIM8_Handler.Init.Prescaler = 0;
    TIM8_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM8_Handler.Init.Period = 0xffff;
    TIM8_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM8_Handler.Init.RepetitionCounter = 0;
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
    sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 5;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 5;

    HAL_TIM_Encoder_Init(&TIM8_Handler, &sConfig);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    HAL_TIMEx_MasterConfigSynchronization(&TIM8_Handler, &sMasterConfig);
    HAL_TIM_Encoder_Start(&TIM8_Handler, TIM_CHANNEL_ALL);

}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim_encoder)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if (htim_encoder->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
    else if (htim_encoder->Instance == TIM8)
    {
        __HAL_RCC_TIM8_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
    }

}

void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim_encoder)
{

    if (htim_encoder->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4 | GPIO_PIN_5);
    }
    else if (htim_encoder->Instance == TIM8)
    {
        __HAL_RCC_TIM8_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOI, GPIO_PIN_5 | GPIO_PIN_6);
    }
}

void setcounterzero(void)
{
    __HAL_TIM_SET_COUNTER(&TIM3_Handler, 0);
    __HAL_TIM_SET_COUNTER(&TIM8_Handler, 0);
}

void getspeedcounter(rt_int16_t *speed1, rt_int16_t *speed2)
{
    *speed1 = __HAL_TIM_GET_COUNTER(&TIM3_Handler);
    *speed2 = 0 - __HAL_TIM_GET_COUNTER(&TIM8_Handler);
}

int hw_ABP_init(void)
{
    TIM_abphase_Init();
    return RT_EOK;
}

INIT_BOARD_EXPORT(hw_ABP_init);
