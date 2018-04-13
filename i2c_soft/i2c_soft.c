#include "i2c_soft.h"

#ifdef __NO_INLINE
  #define __STATIC_INLINE
#else
  #define __STATIC_INLINE       static inline
#endif

sI2C_soft_t sI2C;

__STATIC_INLINE void i2c_soft_start(sI2C_soft_t* psI2C)
{
  psI2C->pfSDA(1);
  psI2C->pfSCL(1);
  psI2C->pfDelay();
  psI2C->pfSDA(0);
  psI2C->pfDelay();
  psI2C->pfSCL(0);
}

__STATIC_INLINE void i2c_soft_stop(sI2C_soft_t* psI2C)
{
  psI2C->pfSDA(0);
  psI2C->pfSCL(1);
  psI2C->pfDelay();
  psI2C->pfSDA(1);
}

__STATIC_INLINE uint8_t i2c_soft_read(sI2C_soft_t* psI2C)
{
  uint8_t       dat = 0;
  uint8_t       i = 0;

  for(; i < 8; i ++) {
    psI2C->pfDelay();
    psI2C->pfSCL(1);
    dat <<= 1;
    if(psI2C->pfSDARead())
      dat |= 0x01;
    psI2C->pfDelay();
    psI2C->pfSCL(0);
  }
  return dat;
}

__STATIC_INLINE void i2c_soft_write(sI2C_soft_t* psI2C, uint8_t dat)
{
  uint8_t       i = 0;

  for(; i < 8; i ++) {
    if(dat & 0x80)
      psI2C->pfSDA(1);
    else
      psI2C->pfSDA(0);
    dat <<= 1;
    psI2C->pfDelay();
    psI2C->pfSCL(1);
    psI2C->pfDelay();
    psI2C->pfSCL(0);
  }
  psI2C->pfSDA(1);
}

__STATIC_INLINE eI2C_soft_ack_t i2c_soft_checkACK(sI2C_soft_t* psI2C)
{
  eI2C_soft_ack_t       eACK;

  psI2C->pfDelay();
  psI2C->pfSCL(1);
  if(psI2C->pfSDARead() == 1)
    eACK = eI2C_SOFT_ACK_NO;
  else
    eACK = eI2C_SOFT_ACK_YES;
  psI2C->pfDelay();
  psI2C->pfSCL(0);
  return eACK;
}

__STATIC_INLINE void i2c_soft_sendACK(sI2C_soft_t* psI2C, eI2C_soft_ack_t eACK)
{
  if(eACK == eI2C_SOFT_ACK_YES)
    psI2C->pfSDA(0);
  else
    psI2C->pfSDA(1);
  psI2C->pfDelay();
  psI2C->pfSCL(1);
  psI2C->pfDelay();
  psI2C->pfSCL(0);
  psI2C->pfSDA(1);
}

void i2c_soft_init(sI2C_soft_t* psI2C, pfPinOperate_t pfSDA, pfPinOperate_t pfSCL, 
              pfSDARead_t pfSDARead, pfDelay_t pfDelay)
{
  psI2C->pfSDA = pfSDA;
  psI2C->pfSCL = pfSCL;
  psI2C->pfSDARead = pfSDARead;
  psI2C->pfDelay = pfDelay;
}

int i2c_soft_writeBytes(sI2C_soft_t* psI2C, uint8_t addr, uint8_t reg, uint8_t* pDat, uint8_t len)
{
  i2c_soft_start(psI2C);
  i2c_soft_write(psI2C, addr << 1);
  if(i2c_soft_checkACK(psI2C) == eI2C_SOFT_ACK_NO)
    return -1;
  i2c_soft_write(psI2C, reg);
  if(i2c_soft_checkACK(psI2C) == eI2C_SOFT_ACK_NO)
    return -1;
  while(len --) {
    i2c_soft_write(psI2C, *pDat);
    if(i2c_soft_checkACK(psI2C) == eI2C_SOFT_ACK_NO)
      return -1;
    pDat ++;
  }
  i2c_soft_stop(psI2C);
  return 0;
}

int i2c_soft_readBytes(sI2C_soft_t* psI2C, uint8_t addr, uint8_t reg, uint8_t* pDat, uint8_t len)
{
  i2c_soft_start(psI2C);
  i2c_soft_write(psI2C, addr << 1);
  if(i2c_soft_checkACK(psI2C) == eI2C_SOFT_ACK_NO)
    return -1;
  i2c_soft_write(psI2C, reg);
  if(i2c_soft_checkACK(psI2C) == eI2C_SOFT_ACK_NO)
    return -1;
  i2c_soft_start(psI2C);
  i2c_soft_write(psI2C, (addr << 1) | 0x01);
  if(i2c_soft_checkACK(psI2C) == eI2C_SOFT_ACK_NO)
    return -1;
  while(len > 1) {
    *pDat = i2c_soft_read(psI2C);
    i2c_soft_sendACK(psI2C, eI2C_SOFT_ACK_YES);
    pDat ++;
    len --;
  }
  if(len == 1) {
    *pDat = i2c_soft_read(psI2C);
    i2c_soft_sendACK(psI2C, eI2C_SOFT_ACK_NO);
  }
  i2c_soft_stop(psI2C);
  return 0;
}
