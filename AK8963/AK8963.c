#include "AK8963.h"

#ifdef __NO_INLINE
  #define __STATIC_INLINE
#else
  #define __STATIC_INLINE       static inline
#endif

sAK8963_t sAK8963;

__STATIC_INLINE void writeByte(sAK8963_t* psAK, uint8_t reg, uint8_t dat)
{
  psAK->pfWrite(psAK->addr, reg, &dat, 1);
}

__STATIC_INLINE void writeBytes(sAK8963_t* psAK, uint8_t reg, uint8_t* pDat, uint16_t len)
{
  psAK->pfWrite(psAK->addr, reg, pDat, len);
}

__STATIC_INLINE uint8_t readByte(sAK8963_t* psAK, uint8_t reg)
{
  uint8_t       buf;
  psAK->pfRead(psAK->addr, reg, &buf, 1);
  return buf;
}

__STATIC_INLINE void readBytes(sAK8963_t* psAK, uint8_t reg, uint8_t* pBuf, uint16_t len)
{
  psAK->pfRead(psAK->addr, reg, pBuf, len);
}

void AK8963_init(sAK8963_t* psAK, uint8_t addr, pfAK8963_transfer_t pfWrite, 
              pfAK8963_transfer_t pfRead, pfAK8963_delayMS_t pfDelay_ms)
{
  psAK->addr = addr;
  psAK->pfWrite = pfWrite;
  psAK->pfRead = pfRead;
  psAK->pfDelayMS = pfDelay_ms;
}

int16_t AK8963_start(sAK8963_t* psAK, uint16_t rate)
{
  if(readByte(psAK, AK8963_RA_WIA) == 0x48) {
    writeByte(psAK, AK8963_RA_CNTL, AK8963_MODE_16BIT | AK8963_MODE_CONT2);  //16bit data, 100hz rate
    return 0;
  }
  return -1;
}

void AK8963_readMagnetic(sAK8963_t* psAK)
{
  uint8_t       buf[8];
  readBytes(psAK, AK8963_RA_HXL - 1, buf, 8);
  psAK->dat.magX = ((int16_t)buf[2] << 8) | buf[1];
  psAK->dat.magY = ((int16_t)buf[4] << 8) | buf[3];
  psAK->dat.magZ = ((int16_t)buf[6] << 8) | buf[5];
}
