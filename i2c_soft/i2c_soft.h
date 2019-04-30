#ifndef I2C_SOFT_H
#define I2C_SOFT_H

#ifdef __cplusplus
extern "C" {
#endif

//other include begin

#include "stdint.h"

//other include end

//#define uint8_t

typedef void        (*pfPinOperate_t)(int level);
typedef int         (*pfSDARead_t)(void);
typedef void        (*pfDelay_t)(void);

typedef enum {
  eI2C_SOFT_ACK_NO,
  eI2C_SOFT_ACK_YES
} eI2C_soft_ack_t;

typedef struct {
  pfPinOperate_t        pfSDA;
  pfPinOperate_t        pfSCL;
  
  pfSDARead_t           pfSDARead;
  pfDelay_t             pfDelay;
} sI2C_soft_t;

void        i2c_soft_init(sI2C_soft_t* psI2C, pfPinOperate_t pfSDA, pfPinOperate_t pfSCL, 
              pfSDARead_t pfSDARead, pfDelay_t pfDelay);

int         i2c_soft_writeBytes(sI2C_soft_t* psI2C, uint8_t addr, uint8_t reg, uint8_t* pDat, uint8_t len);
int         i2c_soft_readBytes(sI2C_soft_t* psI2C, uint8_t addr, uint8_t reg, uint8_t* pDat, uint8_t len);

extern sI2C_soft_t sI2C;

#ifdef __cplusplus
}
#endif

#endif

