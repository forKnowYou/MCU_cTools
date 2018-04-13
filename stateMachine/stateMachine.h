#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdlib.h"

typedef enum {
  eSM_STATUS_OK,
  eSM_STATUS_BUSY,
  eSM_STATUS_DONE
} eSM_status_t;

typedef enum {
  eSM_RW_W        = 0x00,
  eSM_RW_R        = 0x01
} eSM_RW_t;

typedef enum {
  eSM_PROTECT_YES,
  eSM_PROTECT_NO
} eSM_protect_t;

typedef void        (*pfSM_callBack_t)(void* pAgrv);
typedef size_t      (*pfSM_getTick_t)(void);

typedef struct {
  eSM_status_t        eStatus;
  eSM_RW_t            eRW;
  eSM_protect_t       eProtect;
  size_t              tick;

  void*               pCallBackArgv;

  pfSM_callBack_t     pfCallBack;
  pfSM_getTick_t      pfGetTick;
} sSM_base_t;

#ifdef __cplusplus
}
#endif

#endif
