/*
 * File      : drv_mpu6050.c
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
 * 2017-01-08     DQL          first implementation.
 */

#include <rthw.h>
#include <rtthread.h>

#include <rthw.h>
#include <rtdevice.h>

#include "drv_mpu6050.h"
#include "drv_iic.h"


#define MPU6050_I2CBUS_NAME  "i2c2"

#if 1
#define MPUDEBUG      rt_kprintf
#else
#define MPUDEBUG(...)
#endif

/*mpu6050 registers define*/
#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器A
#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG			0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG			0X29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG			0X2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG			0X2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG			0X32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define MPU_INT_STA_REG			0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器
 
#define MPU6050_ADDR				0X68

static struct rt_i2c_bus_device *mpu6050_i2c_bus;
 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//      -1,错误代码
rt_err_t mpu6050_write_reg(rt_uint8_t reg,rt_uint8_t data) 				 
{       
    rt_uint8_t buf[2];
    
    buf[0]=reg;
    buf[1]=data;
       
    if (rt_i2c_master_send(mpu6050_i2c_bus,MPU6050_ADDR,0,buf,2) == 2)
        return RT_EOK;
    else
        return -RT_ERROR;
}
//reg:寄存器地址 
//data:数据指针
//返回值:0,正常
//      -1,错误代码
rt_err_t mpu6050_read_reg(rt_uint8_t reg,rt_uint8_t *data)
{
    struct rt_i2c_msg msgs[2];
               
    msgs[0].addr  = MPU6050_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = MPU6050_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = data;
    msgs[1].len   = 1;

    if (rt_i2c_transfer(mpu6050_i2c_bus, msgs, 2) == 2)
        return RT_EOK;
    else
        return -RT_ERROR;
}

//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//     -1,错误代码
rt_err_t mpu6050_read_regs(rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf)
{ 
	struct rt_i2c_msg msgs[2];
                
    msgs[0].addr  = MPU6050_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = MPU6050_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = len;

    if (rt_i2c_transfer(mpu6050_i2c_bus, msgs, 2) == 2)
    {
        return RT_EOK;        
    }
    else
    {
        return -RT_ERROR;
    }
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//      -1,设置失败 
rt_err_t mpu6050_set_gyro_fsr(rt_uint8_t fsr)
{
	return mpu6050_write_reg(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//     -1,设置失败 
rt_err_t mpu6050_set_accel_fsr(rt_uint8_t fsr)
{
	return mpu6050_write_reg(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//     -1,设置失败 
rt_err_t mpu6050_set_lpf(rt_uint16_t lpf)
{
	rt_uint8_t data = 0;
    
	if (lpf >= 188) data = 1;
	else if (lpf >= 98) data=2;
	else if (lpf >= 42) data=3;
	else if (lpf >= 20) data=4;
	else if (lpf >= 10) data=5;
	else data = 6; 
	return mpu6050_write_reg(MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//      -1,设置失败 
rt_err_t mpu6050_set_sample_rate(rt_uint16_t rate)
{
	rt_uint8_t data;
    
	if (rate > 1000) rate=1000;
	if (rate < 4) rate = 4;
	data = 1000/rate-1;
	data = mpu6050_write_reg(MPU_SAMPLE_RATE_REG,data);	//设置采样率
 	return mpu6050_set_lpf(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//temperature:温度值(扩大了100倍)
//返回值:0,成功
//      -1,失败 
rt_err_t mpu6050_get_temperature(rt_int16_t *temperature)
{
    rt_uint8_t buf[2]; 
    rt_int16_t raw;
	float temp;
    rt_err_t ret;
    
	ret = mpu6050_read_regs(MPU_TEMP_OUTH_REG,2,buf);
    if (ret == RT_EOK) 
    {        
        raw=((rt_uint16_t)buf[0]<<8)|buf[1];  
        temp=36.53+((double)raw)/340;  
        *temperature = temp*100;
        
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//      -1,错误代码
rt_err_t mpu6050_get_gyroscope(rt_int16_t *gx,rt_int16_t *gy,rt_int16_t *gz)
{
    rt_uint8_t buf[6],ret;  
    
	ret = mpu6050_read_regs(MPU_GYRO_XOUTH_REG,6,buf);
	if (ret == 0)
	{
		*gx=((rt_uint16_t)buf[0]<<8)|buf[1];  
		*gy=((rt_uint16_t)buf[2]<<8)|buf[3];  
		*gz=((rt_uint16_t)buf[4]<<8)|buf[5];
        
        return RT_EOK;
	} 	
    else
    {
        return -RT_ERROR;
    } 
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//      -1,错误代码
rt_err_t mpu6050_get_accelerometer(rt_int16_t *ax,rt_int16_t *ay,rt_int16_t *az)
{
    rt_uint8_t buf[6],ret; 
    
	ret = mpu6050_read_regs(MPU_ACCEL_XOUTH_REG,6,buf);
	if (ret == 0)
	{
		*ax=((rt_uint16_t)buf[0]<<8)|buf[1];  
		*ay=((rt_uint16_t)buf[2]<<8)|buf[3];  
		*az=((rt_uint16_t)buf[4]<<8)|buf[5];
        
        return RT_EOK;
	}
    else
    {
        return -RT_ERROR;
    } 	    
}
/* end of mpu6050 driver */

//MPU6050硬件初始化
//返回值:0,成功
//      -1,错误代码
int mpu6050_hw_init(void)
{ 
	rt_uint8_t res;   
    rt_device_t dev;
    
    //i2c初始化
    //hw_i2c_init();
    //after init i2c,must delay a while before init mpu6050
    rt_thread_delay(rt_tick_from_millisecond(100));

    dev = rt_device_find(MPU6050_I2CBUS_NAME);
    
    if (dev == RT_NULL) 
    {
        MPUDEBUG("can't find mpu6050 %s device\r\n",MPU6050_I2CBUS_NAME);
        return -RT_ERROR;
    }
    
    if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR) != RT_EOK)
    {
        MPUDEBUG("can't opend mpu6050 %s device\r\n",MPU6050_I2CBUS_NAME);
        return -RT_ERROR;
    }
    //获取i2c设备句柄
    mpu6050_i2c_bus = (struct rt_i2c_bus_device *)dev;
    
    MPUDEBUG("mpu6050 set i2c bus to %s\r\n", MPU6050_I2CBUS_NAME);
    
    //复位MPU6050           
	mpu6050_write_reg(MPU_PWR_MGMT1_REG,0x80);
    //delay a while
    rt_thread_delay(rt_tick_from_millisecond(20));
    //唤醒MPU6050 
	mpu6050_write_reg(MPU_PWR_MGMT1_REG,0x00);
    //delay a while	
    rt_thread_delay(rt_tick_from_millisecond(20));
    //陀螺仪传感器,±2000dps
	mpu6050_set_gyro_fsr(3);	
    //加速度传感器,±2g
	mpu6050_set_accel_fsr(0);
    //关闭所有中断
	mpu6050_write_reg(MPU_INT_EN_REG,0x00);	
    //I2C主模式关闭
	mpu6050_write_reg(MPU_USER_CTRL_REG,0x00);
    //关闭FIFO
	mpu6050_write_reg(MPU_FIFO_EN_REG,0x00);	
    //INT引脚低电平有效
	mpu6050_write_reg(MPU_INTBP_CFG_REG,0x80);	
           
    //读WHO_AM_I寄存器
	mpu6050_read_reg(MPU_DEVICE_ID_REG, &res);
	if (res == MPU6050_ADDR)
	{       
        MPUDEBUG("read mpu6050 id ok: 0x%02x\r\n",res);
        //设置CLKSEL,PLL X轴为参考
        mpu6050_write_reg(MPU_PWR_MGMT1_REG,0x01);	
        //加速度与陀螺仪都工作
        mpu6050_write_reg(MPU_PWR_MGMT2_REG,0x00);	
        //设置采样率为125Hz
        mpu6050_set_sample_rate(125);				
 	}
    else 
    {     
        MPUDEBUG("read mpu6050 id error: 0x%02x\r\n",res);        
        return -RT_ERROR;
    }
        
    return RT_EOK;
}
INIT_APP_EXPORT(mpu6050_hw_init);




