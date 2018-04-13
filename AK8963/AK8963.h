#ifndef AK8963_H
#define AK8963_H

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

#define AK8963_ADDRESS_00         0x0C
#define AK8963_ADDRESS_01         0x0D
#define AK8963_ADDRESS_10         0x0E // default for InvenSense MPU-6050 evaluation board
#define AK8963_ADDRESS_11         0x0F
#define AK8963_DEFAULT_ADDRESS    AK8963_ADDRESS_00

#define AK8963_RA_WIA             0x00
#define AK8963_RA_INFO            0x01
#define AK8963_RA_ST1             0x02
#define AK8963_RA_HXL             0x03
#define AK8963_RA_HXH             0x04
#define AK8963_RA_HYL             0x05
#define AK8963_RA_HYH             0x06
#define AK8963_RA_HZL             0x07
#define AK8963_RA_HZH             0x08
#define AK8963_RA_ST2             0x09
#define AK8963_RA_CNTL            0x0A
#define AK8963_RA_RSV             0x0B // RESERVED, DO NOT USE
#define AK8963_RA_ASTC            0x0C
#define AK8963_RA_TS1             0x0D // SHIPMENT TEST, DO NOT USE
#define AK8963_RA_TS2             0x0E // SHIPMENT TEST, DO NOT USE
#define AK8963_RA_I2CDIS          0x0F
#define AK8963_RA_ASAX            0x10
#define AK8963_RA_ASAY            0x11
#define AK8963_RA_ASAZ            0x12

#define AK8963_ST1_DRDY_BIT       0

#define AK8963_ST2_HOFL_BIT       3
#define AK8963_ST2_DERR_BIT       2

#define AK8963_CNTL_MODE_BIT      3
#define AK8963_CNTL_MODE_LENGTH   4

#define AK8963_MODE_POWERDOWN     0x00
#define AK8963_MODE_SINGLE        0x01
#define AK8963_MODE_CONT1         0x02
#define AK8963_MODE_CONT2         0x06
#define AK8963_MODE_EXTTRIG       0x04
#define AK8963_MODE_SELFTEST      0x08
#define AK8963_MODE_FUSEROM       0x0F
#define AK8963_MODE_14BIT         0x00
#define AK8963_MODE_16BIT         0x10

typedef int16_t       (* pfAK8963_transfer_t)(uint8_t addr, uint8_t reg, uint8_t* pDat, uint16_t len);
typedef void          (* pfAK8963_delayMS_t)(uint16_t t);

typedef struct {
  int16_t       magX;
  int16_t       magY;
  int16_t       magZ;
} sAK8963_dat_t;

typedef struct {
  uint8_t       addr;

  sAK8963_dat_t dat;

  pfAK8963_transfer_t       pfWrite;
  pfAK8963_transfer_t       pfRead;
  pfAK8963_delayMS_t        pfDelayMS;
} sAK8963_t;

void        AK8963_init(sAK8963_t* psAK, uint8_t addr, pfAK8963_transfer_t pfWrite, 
              pfAK8963_transfer_t pfRead, pfAK8963_delayMS_t pfDelay_ms);
int16_t     AK8963_start(sAK8963_t* psAK, uint16_t rate);

void        AK8963_readMagnetic(sAK8963_t* psAK);
int16_t     AK8963_readMagneticX(sAK8963_t* psAK);
int16_t     AK8963_readMagneticY(sAK8963_t* psAK);
int16_t     AK8963_readMagneticZ(sAK8963_t* psAK);

extern sAK8963_t sAK8963;

#ifdef __cplusplus
}
#endif

#endif
