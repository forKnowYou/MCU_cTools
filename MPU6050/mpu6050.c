#include "mpu6050.h"  

sMPU6050_t        sMPU;

#ifdef __NO_INLINE
  #define __STATIC_INLINE
#else
  #define __STATIC_INLINE       static inline
#endif


__STATIC_INLINE void writeByte(sMPU6050_t* psMPU, uint8_t reg, uint8_t dat)
{
  psMPU->pfWrite(psMPU->addr, reg, &dat, 1);
}

__STATIC_INLINE uint8_t readByte(sMPU6050_t* psMPU, uint8_t reg)
{
  uint8_t       dat;
  psMPU->pfRead(psMPU->addr, reg, &dat, 1);
  return dat;
}

__STATIC_INLINE void readBytes(sMPU6050_t* psMPU, uint8_t reg, uint8_t* pDat, uint16_t len)
{
  psMPU->pfRead(psMPU->addr, reg, pDat, len);
}

void MPU6050_init(sMPU6050_t* psMPU, uint8_t addr, pfMPU6050_transfer_t pfWrite, 
                  pfMPU6050_transfer_t pfRead, pfMPU6050_delayMS_t pfDelayMS)
{
  psMPU->addr = addr;
  psMPU->pfWrite = pfWrite;
  psMPU->pfRead = pfRead;
  psMPU->pfDelayMS = pfDelayMS;
}

int16_t MPU6050_start(sMPU6050_t* psMPU, uint16_t rate)
{
  writeByte(psMPU, MPU6050_PWR_MGMT1_REG, 0x80);  //restart
  psMPU->pfDelayMS(100);
  uint8_t id = readByte(psMPU, MPU6050_DEVICE_ID_REG);  //read id
  if((id == 0x68) || (id == 0x71)) {  //true id
    writeByte(psMPU, MPU6050_PWR_MGMT1_REG, 0X01);  //weak up and set axis x as clock
    psMPU->pfDelayMS(10);
		writeByte(psMPU, MPU6050_PWR_MGMT2_REG, 0X00);  //make accelerometer and gyro work
    MPU6050_setGyroFsr(psMPU, eMPU6050_GYRO_16G);  //set gyro range 16g
    MPU6050_setAccelFsr(psMPU, eMPU6050_ACCEL_2000);  //set accel range 2000 degree
    writeByte(psMPU, MPU6050_INT_EN_REG, 0X00);  //disable all interrupt
  	writeByte(psMPU, MPU6050_USER_CTRL_REG, 0X00);  //disable i2c master mode
  	writeByte(psMPU, MPU6050_FIFO_EN_REG, 0X00);  //disable fifo
  	writeByte(psMPU, MPU6050_INTBP_CFG_REG, 0X02);  //int pin active low, enable bypass mode
  	MPU6050_setRate(psMPU, rate);  //set read rate
    psMPU->pfDelayMS(2);
    return 0;
  }
  return -1;
}

void MPU6050_setGyroFsr(sMPU6050_t* psMPU, uint8_t fsr)
{
  writeByte(psMPU, MPU6050_GYRO_CFG_REG, fsr << 3);
}

void MPU6050_setAccelFsr(sMPU6050_t* psMPU, uint8_t fsr)
{
  writeByte(psMPU, MPU6050_ACCEL_CFG_REG, fsr << 3);
}

void MPU6050_setLPF(sMPU6050_t* psMPU, uint16_t lpf)
{
  uint8_t       data=0;
  if(lpf>=188)
    data=1;
  else if(lpf>=98)
    data=2;
  else if(lpf>=42)
    data=3;
  else if(lpf>=20)
    data=4;
  else if(lpf>=10)
    data=5;
  else
    data=6; 
  writeByte(psMPU, MPU6050_CFG_REG,data);
}

void MPU6050_setRate(sMPU6050_t* psMPU, uint16_t rate)
{
  uint8_t       data;
	if(rate>1000)
    rate = 1000;
	if(rate<4)
    rate = 4;
	data = 1000 / rate - 1;
	psMPU->dataRate = rate;
	writeByte(psMPU, MPU6050_SAMPLE_RATE_REG, data);
 	MPU6050_setLPF(psMPU, rate / 2);
}

void MPU6050_setFIFO(sMPU6050_t* psMPU, uint8_t sens)
{
  
}

void MPU6050_readFIFO(sMPU6050_t* psMPU, uint8_t* pBuf, uint16_t len)
{
  readBytes(psMPU, 0x49, pBuf, len);
}

void MPU6050_ctrlMaster(sMPU6050_t* psMPU, uint8_t ctrl)
{
  writeByte(psMPU, MPU6050_I2CMST_CTRL_REG, ctrl);
}

__STATIC_INLINE uint8_t calcSlave(uint8_t slave)
{
  return MPU6050_I2CSLV0_ADDR_REG + slave * 3;
}

void MPU6050_circleSlave(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg, uint8_t rate, uint8_t len)
{
  uint8_t       slaveBase = calcSlave(slave);
  writeByte(psMPU, MPU6050_FIFO_EN_REG, 0x01 << slave);
  writeByte(psMPU, MPU6050_I2CMST_DELAY_REG, 0x01 << slave);
  writeByte(psMPU, slaveBase, addr | 0x80);
  writeByte(psMPU, slaveBase + 1, reg);
  writeByte(psMPU, MPU6050_I2CSLV4_CTRL_REG, psMPU->dataRate / rate - 1);
  writeByte(psMPU, slaveBase + 2, len | 0x80);
}

void MPU6050_slaveWriteByte(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg, uint8_t dat)
{
  uint8_t       slaveBase = calcSlave(slave);
  readByte(psMPU, MPU6050_INT_STA_REG);
  writeByte(psMPU, slaveBase, addr);
  writeByte(psMPU, slaveBase + 1, reg);
  writeByte(psMPU, MPU6050_I2CSLV0_DO_REG + slave, dat);
  writeByte(psMPU, slaveBase + 2, 0x01 | 0x80);
  while((readByte(psMPU, MPU6050_INT_STA_REG) & 0x08) != 0x08);
  writeByte(psMPU, slaveBase + 2, 0x00);
}

void MPU6050_slaveWriteBytes(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg, uint8_t* pDat, uint16_t len);

uint8_t MPU6050_slaveReadByte(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg)
{
  uint8_t       slaveBase = calcSlave(slave);
  uint8_t       dat;
  readByte(psMPU, MPU6050_INT_STA_REG);
  writeByte(psMPU, slaveBase, addr | 0x80);
  writeByte(psMPU, slaveBase + 1, reg);
  writeByte(psMPU, slaveBase + 2, 0x01 | 0x80);
  while((readByte(psMPU, MPU6050_INT_STA_REG) & 0x08) != 0x08);
  dat = readByte(psMPU, 0x49);
  writeByte(psMPU, slaveBase + 2, 0x00);
  return dat;
}

void MPU6050_slaveReadBytes(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg, uint8_t* pDat, uint16_t len);

uint16_t MPU6050_slaveCheckFIFO(sMPU6050_t* psMPU)
{
  uint8_t       buf[2];
  readBytes(psMPU, MPU6050_FIFO_CNTL_REG, buf, 2);
  return (((uint16_t)buf[1] << 8) | buf[0]);
}

int16_t MPU6050_readTemprature(sMPU6050_t* psMPU)
{
  uint8_t       buf[2]; 
  int16_t       raw;
	float         temp;
	readBytes(psMPU, MPU6050_TEMP_OUTH_REG, buf, 2); 
  raw = ((uint16_t)buf[0] << 8) | buf[1];
  temp = 36.53 + ((double)raw) / 340;
  psMPU->dat.temp = temp * 100;
  return psMPU->dat.temp;
}

void MPU6050_readGyroscope(sMPU6050_t* psMPU)
{
  uint8_t       buf[6] = {0};
  readBytes(psMPU, MPU6050_GYRO_XOUTH_REG, buf, 6);
  psMPU->dat.gyroX = (int16_t)(((int16_t)buf[0] << 8) | ((int16_t)buf[1]));
  psMPU->dat.gyroY = (int16_t)(((int16_t)buf[2] << 8) | ((int16_t)buf[3]));
  psMPU->dat.gyroZ = (int16_t)(((int16_t)buf[4] << 8) | ((int16_t)buf[5]));
}

int16_t MPU6050_readGyroscopeX(sMPU6050_t* psMPU)
{
  uint8_t       buf[2];  
	readBytes(psMPU, MPU6050_GYRO_XOUTH_REG, buf, 2);
  psMPU->dat.gyroX = (((int16_t)buf[0] << 8) | buf[1]) - psMPU->datCali.gyroX;
	return psMPU->dat.gyroX;
}

int16_t MPU6050_readGyroscopeY(sMPU6050_t* psMPU)
{
  uint8_t       buf[2];  
	readBytes(psMPU, MPU6050_GYRO_YOUTH_REG, buf, 2);
  psMPU->dat.gyroY = (((int16_t)buf[0] << 8) | buf[1]) - psMPU->datCali.gyroY;
	return psMPU->dat.gyroY;
}

int16_t MPU6050_readGyroscopeZ(sMPU6050_t* psMPU)
{
  uint8_t       buf[2];  
	readBytes(psMPU, MPU6050_GYRO_ZOUTH_REG, buf, 2);
  psMPU->dat.gyroZ = (((int16_t)buf[0] << 8) | buf[1]) - psMPU->datCali.gyroZ;
	return psMPU->dat.gyroZ;
}

void MPU6050_readAccelerometer(sMPU6050_t* psMPU)
{
  uint8_t       buf[6] = {0};
  readBytes(psMPU, MPU6050_ACCEL_XOUTH_REG, buf, 6);
  psMPU->dat.accelX = (int16_t)(((int16_t)buf[0] << 8) | ((int16_t)buf[1]));
  psMPU->dat.accelY = (int16_t)(((int16_t)buf[2] << 8) | ((int16_t)buf[3]));
  psMPU->dat.accelZ = (int16_t)(((int16_t)buf[4] << 8) | ((int16_t)buf[5]));
}

int16_t MPU6050_readAccelerometerX(sMPU6050_t* psMPU)
{
  uint8_t       buf[2];  
	readBytes(psMPU, MPU6050_ACCEL_XOUTH_REG, buf, 2);
  psMPU->dat.accelX = (((int16_t)buf[0] << 8) | buf[1]) - psMPU->datCali.accelX;
	return psMPU->dat.accelX;
}

int16_t MPU6050_readAccelerometerY(sMPU6050_t* psMPU)
{
  uint8_t       buf[2];  
	readBytes(psMPU, MPU6050_ACCEL_YOUTH_REG, buf, 2);
  psMPU->dat.accelY = (((int16_t)buf[0] << 8) | buf[1]) - psMPU->datCali.accelY;
	return psMPU->dat.accelY;
}

int16_t MPU6050_readAccelerometerZ(sMPU6050_t* psMPU)
{
  uint8_t       buf[2];  
	readBytes(psMPU, MPU6050_ACCEL_ZOUTH_REG, buf, 2);
  psMPU->dat.accelZ = (((int16_t)buf[0] << 8) | buf[1]) - psMPU->datCali.accelZ;
	return psMPU->dat.accelZ;
}
