#include <rthw.h>
#include <rtdevice.h>

#include "app_mpu9250.h"
#include "drv_iic.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"
#include "app_wirelessuart.h"
#include "master_thread.h"

#define MPU9250_I2CBUS_NAME  "i2c0"

#if 1
#define MPUDEBUG      rt_kprintf
#else
#define MPUDEBUG(...)
#endif

static struct rt_i2c_bus_device *mpu9250_i2c_bus;


rt_err_t mpu_dmp_write_Len(rt_uint8_t addr, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *date) //mpu_dmp 移植使用
{
    rt_uint8_t buf[len + 1];
    rt_uint8_t i;
    buf[0] = reg;
    for (i = 0; i < len; i++)
    {
        buf[i + 1] = date[i];
    }
    if (rt_i2c_master_send(mpu9250_i2c_bus, addr, 0, buf, len + 1) == len + 1)
        return RT_EOK;
    else
        return -RT_ERROR;
}


rt_err_t mpu_dmp_read_Len(rt_uint8_t addr, rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf) //mpu_dmp 移植使用
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = len;

    if (rt_i2c_transfer(mpu9250_i2c_bus, msgs, 2) == 2)
        return RT_EOK;
    else
        return -RT_ERROR;
}

//reg:寄存器地址
//data:数据
//返回值:0,正常
//      -1,错误代码
rt_err_t mpu9250_write_reg(rt_uint8_t reg, rt_uint8_t data)
{
    rt_uint8_t buf[2];

    buf[0] = reg;
    buf[1] = data;

    if (rt_i2c_master_send(mpu9250_i2c_bus, MPU9250_ADDR, 0, buf, 2) == 2)
        return RT_EOK;
    else
        return -RT_ERROR;
}
rt_err_t mpu9250_write_magnetometer_reg(rt_uint8_t reg, rt_uint8_t data)
{
    rt_uint8_t buf[2];

    buf[0] = reg;
    buf[1] = data;

    if (rt_i2c_master_send(mpu9250_i2c_bus, AK8963_ADDR, 0, buf, 2) == 2)
        return RT_EOK;
    else
        return -RT_ERROR;
}
//reg:寄存器地址
//data:数据指针
//返回值:0,正常
//      -1,错误代码
rt_err_t mpu9250_read_reg(rt_uint8_t reg, rt_uint8_t *data)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = MPU9250_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = MPU9250_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = data;
    msgs[1].len   = 1;

    if (rt_i2c_transfer(mpu9250_i2c_bus, msgs, 2) == 2)
        return RT_EOK;
    else
        return -RT_ERROR;
}
rt_err_t mpu9250_read_regs(rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = MPU9250_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = MPU9250_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = len;

    if (rt_i2c_transfer(mpu9250_i2c_bus, msgs, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

rt_err_t mpu9250_read_magnetometer_regs(rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = AK8963_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = AK8963_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = buf;
    msgs[1].len   = len;

    if (rt_i2c_transfer(mpu9250_i2c_bus, msgs, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

rt_err_t mpu9250_read_magnetometer_reg(rt_uint8_t reg, rt_uint8_t *data)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = AK8963_ADDR;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = &reg;
    msgs[0].len   = 1;

    msgs[1].addr  = AK8963_ADDR;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf   = data;
    msgs[1].len   = 1;

    if (rt_i2c_transfer(mpu9250_i2c_bus, msgs, 2) == 2)
        return RT_EOK;
    else
        return -RT_ERROR;
}

#include "inv_mpu_dmp_motion_driver.h"
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//      -1,设置失败
rt_err_t mpu9250_set_gyro_fsr(rt_uint8_t fsr)
{
    return mpu9250_write_reg(MPU_GYRO_CFG_REG, fsr << 3); //设置陀螺仪满量程范围
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//     -1,设置失败
rt_err_t mpu9250_set_accel_fsr(rt_uint8_t fsr)
{
    return mpu9250_write_reg(MPU_ACCEL_CFG_REG, fsr << 3); //设置加速度传感器满量程范围
}
//设置MPU9250的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
rt_err_t mpu9250_set_lpf(rt_uint16_t lpf)
{
    rt_uint8_t data = 0;
    if (lpf >= 188)data = 1;
    else if (lpf >= 98)data = 2;
    else if (lpf >= 42)data = 3;
    else if (lpf >= 20)data = 4;
    else if (lpf >= 10)data = 5;
    else data = 6;
    return mpu9250_write_reg(MPU_CFG_REG, data); //设置数字低通滤波器
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//      -1,设置失败
rt_err_t mpu9250_set_sample_rate(rt_uint16_t rate)
{
    rt_uint8_t data;
    if (rate > 1000)rate = 1000;
    if (rate < 4)rate = 4;
    data = 1000 / rate - 1;
    data = mpu9250_write_reg(MPU_SAMPLE_RATE_REG, data);    //设置数字低通滤波器
    return mpu9250_set_lpf(rate / 2);   //自动设置LPF为采样率的一半
}
//rt_err_t MPU9250_write_len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf);
//rt_err_t MPU9250_read_len(rt_uint8_t addr,rt_uint8_t reg,rt_uint8_t len,rt_uint8_t *buf);
//得到温度值
//temperature:温度值(扩大了100倍)
//返回值:0,成功
//      -1,失败
rt_err_t mpu9250_get_temperature(rt_int16_t *temperature)
{
    rt_uint8_t buf[2];
    rt_int16_t raw;
    float temp;
    rt_err_t ret;

    ret = mpu9250_read_regs(MPU_TEMP_OUTH_REG, 2, buf);
    if (ret == RT_EOK)
    {
        raw = ((rt_uint16_t)buf[0] << 8) | buf[1];
        temp = 21 + ((double)raw) / 333.87;
        *temperature = temp * 100;

        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}
rt_err_t mpu9250_get_gyroscope(rt_int16_t *gx, rt_int16_t *gy, rt_int16_t *gz)
{
    rt_uint8_t buf[6], ret;

    ret = mpu9250_read_regs(MPU_GYRO_XOUTH_REG, 6, buf);
    if (ret == 0)
    {
        *gx = ((rt_uint16_t)buf[0] << 8) | buf[1];
        *gy = ((rt_uint16_t)buf[2] << 8) | buf[3];
        *gz = ((rt_uint16_t)buf[4] << 8) | buf[5];

        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}
rt_err_t mpu9250_get_accelerometer(rt_int16_t *ax, rt_int16_t *ay, rt_int16_t *az)
{
    rt_uint8_t buf[6], ret;

    ret = mpu9250_read_regs(MPU_ACCEL_XOUTH_REG, 6, buf);
    if (ret == 0)
    {
        *ax = ((rt_uint16_t)buf[0] << 8) | buf[1];
        *ay = ((rt_uint16_t)buf[2] << 8) | buf[3];
        *az = ((rt_uint16_t)buf[4] << 8) | buf[5];

        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}
rt_err_t mpu9250_get_magnetometer(rt_int16_t *mx, rt_int16_t *my, rt_int16_t *mz)
{
    rt_uint8_t buf[6], ret;

    ret = mpu9250_read_magnetometer_regs(MAG_XOUT_L, 6, buf);
    if (ret == 0)
    {
        *mx = ((rt_uint16_t)buf[0] << 8) | buf[1];
        *my = ((rt_uint16_t)buf[2] << 8) | buf[3];
        *mz = ((rt_uint16_t)buf[4] << 8) | buf[5];

        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0X01~0X1C
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(rt_uint8_t fun, rt_uint8_t *data, rt_uint8_t len)
{
    rt_uint8_t send_buf[32];
    rt_uint8_t i;
    if (len > 28)return; //最多28字节数据
    send_buf[len + 3] = 0;  //校验数置零
    send_buf[0] = 0XAA; //帧头
    send_buf[1] = 0XAA; //帧头
    send_buf[2] = fun;  //功能字
    send_buf[3] = len;  //数据长度
    for (i = 0; i < len; i++)send_buf[4 + i] = data[i];         //复制数据
    for (i = 0; i < len + 4; i++)send_buf[len + 4] += send_buf[i];  //计算校验和
    for (i = 0; i < len + 5; i++)wireless_putchar(send_buf[i]); //发送数据到串口1
}
//发送加速度传感器数据+陀螺仪数据(传感器帧)
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz)
{
    rt_uint8_t tbuf[18];
    tbuf[0] = (aacx >> 8) & 0XFF;
    tbuf[1] = aacx & 0XFF;
    tbuf[2] = (aacy >> 8) & 0XFF;
    tbuf[3] = aacy & 0XFF;
    tbuf[4] = (aacz >> 8) & 0XFF;
    tbuf[5] = aacz & 0XFF;
    tbuf[6] = (gyrox >> 8) & 0XFF;
    tbuf[7] = gyrox & 0XFF;
    tbuf[8] = (gyroy >> 8) & 0XFF;
    tbuf[9] = gyroy & 0XFF;
    tbuf[10] = (gyroz >> 8) & 0XFF;
    tbuf[11] = gyroz & 0XFF;
    tbuf[12] = 0; //因为开启MPL后,无法直接读取磁力计数据,所以这里直接屏蔽掉.用0替代.
    tbuf[13] = 0;
    tbuf[14] = 0;
    tbuf[15] = 0;
    tbuf[16] = 0;
    tbuf[17] = 0;
    usart1_niming_report(0X02, tbuf, 18); //传感器帧,0X02
}
//通过串口1上报结算后的姿态数据给电脑(状态帧)
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
//csb:超声波高度,单位:cm
//prs:气压计高度,单位:mm
void usart1_report_imu(short roll, short pitch, short yaw, short csb, int prs)
{
    rt_uint8_t tbuf[12];
    tbuf[0] = (roll >> 8) & 0XFF;
    tbuf[1] = roll & 0XFF;
    tbuf[2] = (pitch >> 8) & 0XFF;
    tbuf[3] = pitch & 0XFF;
    tbuf[4] = (yaw >> 8) & 0XFF;
    tbuf[5] = yaw & 0XFF;
    tbuf[6] = (csb >> 8) & 0XFF;
    tbuf[7] = csb & 0XFF;
    tbuf[8] = (prs >> 24) & 0XFF;
    tbuf[9] = (prs >> 16) & 0XFF;
    tbuf[10] = (prs >> 8) & 0XFF;
    tbuf[11] = prs & 0XFF;
    usart1_niming_report(0X01, tbuf, 12); //状态帧,0X01
}

void mpu9250_thread_entry(void *parameter)
{
    float pitch, roll, yaw;           //欧拉角
    while (1)
    {
        if (mpu_mpl_get_data(&pitch, &roll, &yaw) == 0)
        {
						rt_mutex_take(&Mpu9250Mutex, RT_WAITING_FOREVER);
						mypitch=pitch;
						myroll=roll;
						myyaw=yaw;
					  printf("myyaw:%f \r\n",myyaw);
//            usart1_report_imu((int)(roll * 100), (int)(pitch * 100), (int)(yaw * 100), 0, 0);
						rt_mutex_release(&Mpu9250Mutex);
        }
        rt_thread_delay(rt_tick_from_millisecond(20));
    }
}

#include "inv_mpu.h"
int mpu9250_init(void)
{
    rt_uint8_t res;
    rt_device_t dev;
    rt_thread_delay(rt_tick_from_millisecond(100));

    dev = rt_device_find(MPU9250_I2CBUS_NAME);

    if (dev == RT_NULL)
    {
        MPUDEBUG("can't find mpu9250 %s device\r\n", MPU9250_I2CBUS_NAME);
        return -RT_ERROR;
    }

    if (rt_device_open(dev, RT_DEVICE_OFLAG_RDWR) != RT_EOK)
    {
        MPUDEBUG("can't opend mpu9250 %s device\r\n", MPU9250_I2CBUS_NAME);
        return -RT_ERROR;
    }
    //获取i2c设备句柄
    mpu9250_i2c_bus = (struct rt_i2c_bus_device *)dev;

    MPUDEBUG("mpu_dmp_init is runing !\r\n");

    while (mpu_dmp_init())
    {
        MPUDEBUG("mpu_dmp_init waiting  ! 0x%02x\r\n", res);
        rt_thread_delay(rt_tick_from_millisecond(100));
    }
    MPUDEBUG("mpu_dmp_init is ok !\r\n");

    rt_thread_t tid;
    /* 创建 MPU9250 线程 *//////////////////////////////////
    tid = rt_thread_create("mpu9250",
                           mpu9250_thread_entry,
                           RT_NULL,
                           2048,
                           3,
                           20);
    /* 创建成功则启动线程 */
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return RT_EOK;

}
