#ifndef __MAT_CMD_H
#define __MAT_CMD_H

#include "stdint.h"
#include "string.h"

typedef void        (* pfNull)(void);
typedef void        (* pfSetCmd)(char* pDat);

typedef struct {
  char*       pName;
  uint8_t     nameLen;
  
  pfNull      pfTest;
  pfNull      pfQuery;
  pfSetCmd    pfSet;
  pfNull      pfExe;
} MAT_OPT_t;

typedef struct {
  MAT_OPT_t*      pOpt;
  uint16_t        len;
  pfNull          pfError;
} MAT_CMD_t;

void        MAT_init(MAT_CMD_t* pCmd ,MAT_OPT_t* pOpt, uint16_t len, pfNull pfError);
void        MAT_receive(MAT_CMD_t* pCmd, char* pDat);

#endif
