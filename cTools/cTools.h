#ifndef CTOOLS_H
#define CTOOLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "stdint.h"

#define MAX_IN_ARRAY(buf, len, rslt, sub) \
          rslt = buf[0]; \
          for(int var_i = 1; var_i < len; var_i ++) { \
            if(rslt < buf[var_i]) \
              rslt = buf[var_i]; \
              sub = var_i; \
          }
          
#define MIN_IN_ARRAY(buf, len, rslt, sub) \
          rslt = buf[0]; \
          for(int var_i = 1; var_i < len; var_i ++) { \
            if(rslt > buf[var_i]) \
              rslt = buf[var_i]; \
              sub = var_i; \
          }

typedef int       (*pfPrint_t)(const char* pCh);

typedef struct {
  char*       pBuf;
  int         count;
  pfPrint_t   pfPrint;
} sCPrintf_t;

void        cPrintfFlush(sCPrintf_t* sCP);
void        cPrintfInit(sCPrintf_t* sCP, char* pBuf, pfPrint_t pfPrint);
void        cPrintfToBuf(sCPrintf_t* sCP, char ch);

extern sCPrintf_t        sCPrintf;

#ifdef __cplusplus
}
#endif

#endif
