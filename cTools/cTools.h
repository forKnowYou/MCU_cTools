#ifndef CTOOLS_H
#define CTOOLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "stdint.h"

typedef int       (*pfPrint_t)(const char* pCh);

typedef struct {
  char*       pBuf;
  int         count;
  pfPrint_t   pfPrint;
} sCPrintf_t;

typedef enum {
  eBASETYPE_INT8,
  eBASETYPE_UINT8,
  eBASETYPE_INT16,
  eBASETYPE_UINT16,
  eBASETYPE_INT32,
  eBASETYPE_UINT32,
  eBASETYPE_FLOAT,
  eBASETYPE_INT64,
  eBASETYPE_UINT64,
  eBASETYPE_DOUBLE
} eBaseType_t;

void        cPrintfFlush(sCPrintf_t* sCP);
void        cPrintfInit(sCPrintf_t* sCP, char* pBuf, pfPrint_t pfPrint);
void        cPrintfToBuf(sCPrintf_t* sCP, char ch);

char*       cFindArrayMaxAddr(char* pBuf, eBaseType_t size, uint16_t len);
char*       cFindArrayMinAddr(char* pBuf, eBaseType_t size, uint16_t len);

extern sCPrintf_t        sCPrintf;

#ifdef __cplusplus
}
#endif

#endif
