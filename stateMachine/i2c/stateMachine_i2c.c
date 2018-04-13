#include "stateMachine_i2c.h"

sStateM_I2C_t       sSM_I2C;

void SM_I2C_init(sStateM_I2C_t* psSM, pfSM_I2C_start_t pfStart, 
                        pfSM_I2C_write_t pfWrite, pfSM_I2C_readDat_t pfReadDat, 
                        pfSM_I2C_stop_t pfStop, pfSM_I2C_getEvent_t pfGetEvent, 
                        pfSM_I2C_setACK_t pfSetACK, pfSM_I2C_getBus_t pfGetBus)
{
  psSM->func.pfGetEvent = pfGetEvent;
  psSM->func.pfStart = pfStart;
  psSM->func.pfWrite = pfWrite;
  psSM->func.pfStop = pfStop;
  psSM->func.pfReadDat = pfReadDat;
  psSM->func.pfSetACK = pfSetACK;
  psSM->func.pfGetBus = pfGetBus;
}

eSM_I2C_bus_t SM_I2C_start(sStateM_I2C_t* psSM, eSM_RW_t eRW, uint8_t id, uint8_t reg, 
                         uint8_t* pData, uint16_t len, uint16_t timeOut)
{
  eSM_I2C_bus_t       eBus = eSM_I2C_BUS_BUSY;
  uint32_t            tick = psSM->base.pfGetTick();
  while((psSM->base.pfGetTick() - tick) < timeOut) {
    eBus = psSM->func.pfGetBus();
    if(eBus == eSM_I2C_BUS_OK) {
      psSM->eLastEvent = eSM_I2C_EVENT_NULL;
      psSM->base.eRW = eRW;
      psSM->id = id << 1;
      psSM->reg = reg;
      psSM->pData = pData;
      psSM->len = len;
      psSM->eStep = eSM_I2C_STEP_1;
      psSM->base.eStatus = eSM_STATUS_BUSY;
      psSM->func.pfSetACK(eSM_I2C_ACK_NO);
      psSM->func.pfStart();
      return eBus;
    }
  }
  return eBus;
}

void SM_I2C_run(sStateM_I2C_t* psSM)
{
  psSM->eEvent = psSM->func.pfGetEvent();
  switch(psSM->eEvent) {
    case eSM_I2C_EVENT_NULL: break;
    case eSM_I2C_EVENT_START: psSM->func.pfWrite(psSM->id | psSM->base.eRW); break;
    case eSM_I2C_EVENT_ADDR: {
      if(psSM->base.eRW == eSM_RW_W)
        psSM->func.pfWrite(psSM->reg);
      else if(psSM->len > 1)
        psSM->func.pfSetACK(eSM_I2C_ACK_YES);
      else if(psSM->len == 1)
        psSM->func.pfStop();
      else {
        psSM->func.pfStop();
        psSM->base.eStatus = eSM_STATUS_DONE;
      }
    } break;
    case eSM_I2C_EVENT_READ: {
      psSM->len --;
      if(psSM->len == 1) {
        psSM->func.pfSetACK(eSM_I2C_ACK_NO);
        psSM->func.pfStop();
      }
      if(psSM->len == 0)
        psSM->base.eStatus = eSM_STATUS_DONE;
      *psSM->pData = psSM->func.pfReadDat();
      psSM->pData ++;
    } break;
    case eSM_I2C_EVENT_WRITE: {
      if(psSM->len > 0) {
        psSM->func.pfWrite(*psSM->pData);
        psSM->pData ++;
        psSM->len --;
      } else {
        psSM->func.pfStop();
        psSM->base.eStatus = eSM_STATUS_DONE;
      }
    } break;
  }
}

void SM_I2C_setRestart(sStateM_I2C_t* psSM, pfSM_I2C_restart_t pfRestart)
{
  psSM->func.pfRestart = pfRestart;
}

void SM_I2C_restart(sStateM_I2C_t* psSM)
{
  psSM->func.pfRestart();
  if(psSM->len != 0) {
    psSM->func.pfStart();
  }
}

void SM_I2C_setBase(sStateM_I2C_t* psSM, pfSM_getTick_t pfGetTick)
{
  psSM->base.pfGetTick = pfGetTick;
}

eSM_status_t SM_I2C_getStatus(sStateM_I2C_t* psSM)
{
  return psSM->base.eStatus;
}

void SM_I2C_setCallBack(sStateM_I2C_t* psSM, pfSM_callBack_t pfCallBack, void* pArgv)
{
  psSM->base.pfCallBack = pfCallBack;
  psSM->base.pCallBackArgv = pArgv;
}

