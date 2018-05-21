#include "cTools.h"

sCPrintf_t        sCPrintf;

void cPrintfFlush(sCPrintf_t* sCP)
{
  char*       pCh = sCP->pBuf;
  sCP->count = 0;
  sCP->pfPrint(sCP->pBuf);
  while(*pCh != 0) {
    *pCh = 0;
    pCh ++;
  }
}

void cPrintfInit(sCPrintf_t* sCP, char* pBuf, pfPrint_t pfPrint)
{
  sCP->pBuf = pBuf;
  sCP->pfPrint = pfPrint;
}

void cPrintfToBuf(sCPrintf_t* sCP, char ch)
{
  sCP->pBuf[sCP->count ++] = ch;
}
