#ifndef STATEMACHINE_I2C_H
#define STATEMACHINE_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stateMachine.h"

typedef enum {
  eSM_I2C_EVENT_NULL,
  eSM_I2C_EVENT_START,
  eSM_I2C_EVENT_ADDR,
  eSM_I2C_EVENT_WRITE,
  eSM_I2C_EVENT_READ
} eSM_I2C_event_t;

typedef enum {
  eSM_I2C_STEP_1,
  eSM_I2C_STEP_2,
  eSM_I2C_STEP_3,
  eSM_I2C_STEP_4,
  eSM_I2C_STEP_5
} eSM_I2C_step_t;

typedef enum {
  eSM_I2C_ACK_YES,
  eSM_I2C_ACK_NO,
  eSM_I2C_ACK_UNKNOW
} eSM_I2C_ACK_t;

typedef enum {
  eSM_I2C_BUS_OK,
  eSM_I2C_BUS_BUSY
} eSM_I2C_bus_t;

typedef void        (*pfSM_I2C_start_t)(void);
typedef void        (*pfSM_I2C_write_t)(uint8_t dat);
typedef void        (*pfSM_I2C_stop_t)(void);
typedef uint8_t     (*pfSM_I2C_readDat_t)(void);
typedef void        (*pfSM_I2C_setACK_t)(eSM_I2C_ACK_t eACK);

typedef eSM_I2C_event_t       (*pfSM_I2C_getEvent_t)(void);
typedef eSM_I2C_bus_t         (*pfSM_I2C_getBus_t)(void);

typedef void        (*pfSM_I2C_restart_t)(void);

typedef struct {
  pfSM_I2C_getEvent_t       pfGetEvent;
  pfSM_I2C_getBus_t         pfGetBus;
  pfSM_I2C_start_t          pfStart;
  pfSM_I2C_write_t          pfWrite;
  pfSM_I2C_readDat_t        pfReadDat;
  pfSM_I2C_stop_t           pfStop;
  pfSM_I2C_setACK_t         pfSetACK;

  pfSM_I2C_restart_t        pfRestart;
} sSM_I2C_func_t;

typedef struct {
  sSM_base_t          base;

  eSM_I2C_event_t     eEvent;
  eSM_I2C_event_t     eLastEvent;
  eSM_I2C_step_t      eStep;
  uint8_t*            pData;
  uint8_t             id;
  uint8_t             reg;
  uint16_t            len;
  
  sSM_I2C_func_t      func;
} sStateM_I2C_t;

void        SM_I2C_init(sStateM_I2C_t* psSM, pfSM_I2C_start_t pfStart, 
                        pfSM_I2C_write_t pfWrite, pfSM_I2C_readDat_t pfReadDat, 
                        pfSM_I2C_stop_t pfStop, pfSM_I2C_getEvent_t pfGetEvent, 
                        pfSM_I2C_setACK_t pfSetACK, pfSM_I2C_getBus_t pfGetBus);
eSM_I2C_bus_t        SM_I2C_start(sStateM_I2C_t* psSM, eSM_RW_t eRW, uint8_t id, uint8_t reg, 
                         uint8_t* pData, uint16_t len, uint16_t timeOut);
void        SM_I2C_run(sStateM_I2C_t* psSM);

void        SM_I2C_setRestart(sStateM_I2C_t* psSM, pfSM_I2C_restart_t pfRestart);
void        SM_I2C_restart(sStateM_I2C_t* psSM);
void        SM_I2C_setCallBack(sStateM_I2C_t* psSM, pfSM_callBack_t pfCallBack, void* pArgv);
void        SM_I2C_setBase(sStateM_I2C_t* psSM, pfSM_getTick_t pfGetTick);

eSM_status_t        SM_I2C_getStatus(sStateM_I2C_t* psSM);

extern sStateM_I2C_t        sSM_I2C;

#ifdef __cplusplus
}
#endif

#endif
