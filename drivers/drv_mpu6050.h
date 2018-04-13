/*
 * File      : drv_mpu6050.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-01-08     DQL        first implementation.
 */

#ifndef __DRV_MPU6050_H__
#define __DRV_MPU6050_H__

#include <rthw.h>
#include <rtthread.h>

#include <rthw.h>
#include <rtdevice.h>

#include "drv_iic.h"

rt_err_t mpu6050_write_reg(rt_uint8_t reg,rt_uint8_t data);
rt_err_t mpu6050_read_reg(rt_uint8_t reg,rt_uint8_t *data);
rt_err_t mpu6050_read_regs(rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf);
rt_err_t mpu6050_set_gyro_fsr(rt_uint8_t fsr);
rt_err_t mpu6050_set_accel_fsr(rt_uint8_t fsr);
rt_err_t mpu6050_set_lpf(rt_uint16_t lpf);
rt_err_t mpu6050_set_sample_rate(rt_uint16_t rate);
rt_err_t mpu6050_get_temperature(rt_int16_t *temperature);
rt_err_t mpu6050_get_gyroscope(rt_int16_t *gx,rt_int16_t *gy,rt_int16_t *gz);
rt_err_t mpu6050_get_accelerometer(rt_int16_t *ax,rt_int16_t *ay,rt_int16_t *az);
int mpu6050_hw_init(void);

#endif
