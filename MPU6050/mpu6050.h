#ifndef MPU6050_H
#define MPU6050_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __C8051
  typedef uint8_t       unsigned char;
  typedef uint16_t      unsigned int;
  typedef int16_t       int;
#else
#include "stdint.h"
#endif

#define MPU6050_ADDR_SA0        0x68
#define MPU6050_ADDR_SA1        0x69

//#define MPU6050_ACCEL_OFFS_REG		0X06
//#define MPU6050_PROD_ID_REG			0X0C	
#define MPU6050_SELF_TESTX_REG		0X0D	
#define MPU6050_SELF_TESTY_REG		0X0E	
#define MPU6050_SELF_TESTZ_REG		0X0F	
#define MPU6050_SELF_TESTA_REG		0X10	
#define MPU6050_SAMPLE_RATE_REG		0X19	
#define MPU6050_CFG_REG				0X1A	
#define MPU6050_GYRO_CFG_REG		0X1B	
#define MPU6050_ACCEL_CFG_REG		0X1C	
#define MPU6050_MOTION_DET_REG		0X1F
#define MPU6050_FIFO_EN_REG			0X23	
#define MPU6050_I2CMST_CTRL_REG		0X24
#define MPU6050_I2CSLV0_ADDR_REG	0X25
#define MPU6050_I2CSLV0_REG			0X26	
#define MPU6050_I2CSLV0_CTRL_REG	0X27
#define MPU6050_I2CSLV1_ADDR_REG	0X28
#define MPU6050_I2CSLV1_REG			0X29	
#define MPU6050_I2CSLV1_CTRL_REG	0X2A
#define MPU6050_I2CSLV2_ADDR_REG	0X2B
#define MPU6050_I2CSLV2_REG			0X2C	
#define MPU6050_I2CSLV2_CTRL_REG	0X2D
#define MPU6050_I2CSLV3_ADDR_REG	0X2E
#define MPU6050_I2CSLV3_REG			0X2F	
#define MPU6050_I2CSLV3_CTRL_REG	0X30
#define MPU6050_I2CSLV4_ADDR_REG	0X31
#define MPU6050_I2CSLV4_REG			0X32	
#define MPU6050_I2CSLV4_DO_REG		0X33
#define MPU6050_I2CSLV4_CTRL_REG	0X34
#define MPU6050_I2CSLV4_DI_REG		0X35

#define MPU6050_I2CMST_STA_REG		0X36
#define MPU6050_INTBP_CFG_REG		0X37	
#define MPU6050_INT_EN_REG			0X38	
#define MPU6050_INT_STA_REG			0X3A	

#define MPU6050_ACCEL_XOUTH_REG		0X3B
#define MPU6050_ACCEL_XOUTL_REG		0X3C
#define MPU6050_ACCEL_YOUTH_REG		0X3D
#define MPU6050_ACCEL_YOUTL_REG		0X3E
#define MPU6050_ACCEL_ZOUTH_REG		0X3F
#define MPU6050_ACCEL_ZOUTL_REG		0X40

#define MPU6050_TEMP_OUTH_REG		0X41	
#define MPU6050_TEMP_OUTL_REG		0X42	

#define MPU6050_GYRO_XOUTH_REG		0X43
#define MPU6050_GYRO_XOUTL_REG		0X44
#define MPU6050_GYRO_YOUTH_REG		0X45
#define MPU6050_GYRO_YOUTL_REG		0X46
#define MPU6050_GYRO_ZOUTH_REG		0X47
#define MPU6050_GYRO_ZOUTL_REG		0X48

#define MPU6050_I2CSLV0_DO_REG		0X63
#define MPU6050_I2CSLV1_DO_REG		0X64
#define MPU6050_I2CSLV2_DO_REG		0X65
#define MPU6050_I2CSLV3_DO_REG		0X66

#define MPU6050_I2CMST_DELAY_REG	0X67
#define MPU6050_SIGPATH_RST_REG		0X68
#define MPU6050_MDETECT_CTRL_REG	0X69
#define MPU6050_USER_CTRL_REG		0X6A	
#define MPU6050_PWR_MGMT1_REG		0X6B	
#define MPU6050_PWR_MGMT2_REG		0X6C	
#define MPU6050_FIFO_CNTH_REG		0X72	
#define MPU6050_FIFO_CNTL_REG		0X73	
#define MPU6050_FIFO_RW_REG			0X74	
#define MPU6050_DEVICE_ID_REG		0X75

typedef int16_t        (* pfMPU6050_transfer_t)(uint8_t addr, uint8_t reg, uint8_t* pDat, uint16_t len);
typedef void           (* pfMPU6050_delayMS_t)(uint16_t t);

typedef enum {
  eMPU6050_SLV_RATE_348,
  eMPU6050_SLV_RATE_333,
  eMPU6050_SLV_RATE_320,
  eMPU6050_SLV_RATE_308,
  eMPU6050_SLV_RATE_296,
  eMPU6050_SLV_RATE_286,
  eMPU6050_SLV_RATE_276,
  eMPU6050_SLV_RATE_267,
  eMPU6050_SLV_RATE_258,
  eMPU6050_SLV_RATE_500,
  eMPU6050_SLV_RATE_471,
  eMPU6050_SLV_RATE_444,
  eMPU6050_SLV_RATE_421,
  eMPU6050_SLV_RATE_400,
  eMPU6050_SLV_RATE_381,
  eMPU6050_SLV_RATE_364
} eMPU6050_SLV_rate_t;

typedef enum {
  eMPU6050_SLV_RW_W = 0x00,
  eMPU6050_SLV_RW_R = 0x80
} eMPU6050_SLV_RW_t;

typedef enum {
  eMPU6050_GYRO_2G,
  eMPU6050_GYRO_4G,
  eMPU6050_GYRO_8G,
  eMPU6050_GYRO_16G
} eMPU6050_gyro_t;

typedef enum {
  eMPU6050_ACCEL_250,
  eMPU6050_ACCEL_500,
  eMPU6050_ACCEL_1000,
  eMPU6050_ACCEL_2000
} eMPU6050_accel_t;

typedef struct {
  int16_t       temp;
  int16_t       gyroX;
  int16_t       gyroY;
  int16_t       gyroZ;
  int16_t       accelX;
  int16_t       accelY;
  int16_t       accelZ;
} MPU6050_dat_t;

typedef struct {
  uint8_t       addr;

  MPU6050_dat_t       dat;
  MPU6050_dat_t       datCali;

  uint16_t      dataRate;

  pfMPU6050_transfer_t        pfWrite;
  pfMPU6050_transfer_t        pfRead;
  pfMPU6050_delayMS_t         pfDelayMS;
} sMPU6050_t;

void        MPU6050_init(sMPU6050_t* psMPU, uint8_t addr, pfMPU6050_transfer_t pfWrite, 
                         pfMPU6050_transfer_t pfRead, pfMPU6050_delayMS_t pfDelayMS);
int16_t     MPU6050_start(sMPU6050_t* psMPU, uint16_t rate);

void        MPU6050_setGyroFsr(sMPU6050_t* psMPU, uint8_t fsr);
void        MPU6050_setAccelFsr(sMPU6050_t* psMPU, uint8_t fsr);
void        MPU6050_setLPF(sMPU6050_t* psMPU, uint16_t lpf);
void        MPU6050_setRate(sMPU6050_t* psMPU, uint16_t rate);
void        MPU6050_setFIFO(sMPU6050_t* psMPU, uint8_t sens);
void        MPU6050_readFIFO(sMPU6050_t* psMPU, uint8_t* pBuf, uint16_t len);

void        MPU6050_ctrlMaster(sMPU6050_t* psMPU, uint8_t ctrl);
void        MPU6050_circleSlave(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg, uint8_t rate, uint8_t len);
void        MPU6050_slaveWriteByte(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg, uint8_t dat);
void        MPU6050_slaveWriteBytes(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg, uint8_t* pDat, uint16_t len);
uint8_t     MPU6050_slaveReadByte(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg);
void        MPU6050_slaveReadBytes(sMPU6050_t* psMPU, uint8_t slave, uint8_t addr, uint8_t reg, uint8_t* pDat, uint16_t len);
uint16_t    MPU6050_slaveCheckFIFO(sMPU6050_t* psMPU);

int16_t     MPU6050_readTemprature(sMPU6050_t* psMPU);
void        MPU6050_readGyroscope(sMPU6050_t* psMPU);
int16_t     MPU6050_readGyroscopeX(sMPU6050_t* psMPU);
int16_t     MPU6050_readGyroscopeY(sMPU6050_t* psMPU);
int16_t     MPU6050_readGyroscopeZ(sMPU6050_t* psMPU);
void        MPU6050_readAccelerometer(sMPU6050_t* psMPU);
int16_t     MPU6050_readAccelerometerX(sMPU6050_t* psMPU);
int16_t     MPU6050_readAccelerometerY(sMPU6050_t* psMPU);
int16_t     MPU6050_readAccelerometerZ(sMPU6050_t* psMPU);

extern sMPU6050_t sMPU;

#ifdef __cplusplus
}
#endif

#endif

