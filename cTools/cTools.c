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

char* cFindArrayMaxAddr(char* pBuf, eBaseType_t size, uint16_t len)
{
  char*       pM = pBuf;
  int8_t*     pI8;
  uint8_t*    pU8;
  int16_t*    pI16;
  uint16_t*   pU16;
  int32_t*    pI32;
  uint32_t*   pU32;
  float*      pF;
  int64_t*    pI64;
  uint64_t*   pU64;
  double*     pD;
  
  switch(size) {
    case eBASETYPE_INT8: {
      pI8 = (int8_t*)pBuf + 1;
      while(len > 1) {
        if(*(int8_t*)pM < *pI8)
          pM = (char*)pI8;
        pI8 ++;
        len --;
      }
    } break;
    case eBASETYPE_UINT8: {
      pU8 = (uint8_t*)pBuf + 1;
      while(len > 1) {
        if(*(uint8_t*)pM < *pU8)
          pM = (char*)pU8;
        pU8 ++;
        len --;
      }
    } break;
    case eBASETYPE_INT16: {
      pI16 = (int16_t*)pBuf + 1;
      while(len > 1) {
        if(*(int16_t*)pM < *pI16)
          pM = (char*)pI16;
        pI16 ++;
        len --;
      }
    } break;
    case eBASETYPE_UINT16: {
      pU16 = (uint16_t*)pBuf + 1;
      while(len > 1) {
        if(*(uint16_t*)pM < *pU16)
          pM = (char*)pU16;
        pU16 ++;
        len --;
      }
    } break;
    case eBASETYPE_INT32: {
      pI32 = (int32_t*)pBuf + 1;
      while(len > 1) {
        if(*(int32_t*)pM < *pI32)
          pM = (char*)pI32;
        pI32 ++;
        len --;
      }
    } break;
    case eBASETYPE_UINT32: {
      pU32 = (uint32_t*)pBuf + 1;
      while(len > 1) {
        if(*(uint32_t*)pM < *pU32)
          pM = (char*)pU32;
        pU32 ++;
        len --;
      }
    } break;
    
    case eBASETYPE_FLOAT: {
      pF = (float*)pBuf + 1;
      while(len > 1) {
        if(*(float*)pM < *pF)
          pM = (char*)pF;
        pF ++;
        len --;
      }
    } break;
    
    case eBASETYPE_INT64: {
      pI64 = (int64_t*)pBuf + 1;
      while(len > 1) {
        if(*(int64_t*)pM < *pI64)
          pM = (char*)pI64;
        pI64 ++;
        len --;
      }
    } break;
    case eBASETYPE_UINT64: {
      pU64 = (uint64_t*)pBuf + 1;
      while(len > 1) {
        if(*(uint64_t*)pM < *pU64)
          pM = (char*)pU64;
        pU64 ++;
        len --;
      }
    } break;
    
    case eBASETYPE_DOUBLE: {
      pD = (double*)pBuf + 1;
      while(len > 1) {
        if(*(double*)pM < *pD)
          pM = (char*)pD;
        pD ++;
        len --;
      }
    } break;
  }
  return pM;
}

char* cFindArrayMinAddr(char* pBuf, eBaseType_t size, uint16_t len)
{
  char*       pM = pBuf;
  int8_t*     pI8;
  uint8_t*    pU8;
  int16_t*    pI16;
  uint16_t*   pU16;
  int32_t*    pI32;
  uint32_t*   pU32;
  float*      pF;
  int64_t*    pI64;
  uint64_t*   pU64;
  double*     pD;
  
  switch(size) {
    case eBASETYPE_INT8: {
      pI8 = (int8_t*)pBuf + 1;
      while(len > 1) {
        if(*(int8_t*)pM > *pI8)
          pM = (char*)pI8;
        pI8 ++;
        len --;
      }
    } break;
    case eBASETYPE_UINT8: {
      pU8 = (uint8_t*)pBuf + 1;
      while(len > 1) {
        if(*(uint8_t*)pM > *pU8)
          pM = (char*)pU8;
        pU8 ++;
        len --;
      }
    } break;
    case eBASETYPE_INT16: {
      pI16 = (int16_t*)pBuf + 1;
      while(len > 1) {
        if(*(int16_t*)pM > *pI16)
          pM = (char*)pI16;
        pI16 ++;
        len --;
      }
    } break;
    case eBASETYPE_UINT16: {
      pU16 = (uint16_t*)pBuf + 1;
      while(len > 1) {
        if(*(uint16_t*)pM > *pU16)
          pM = (char*)pU16;
        pU16 ++;
        len --;
      }
    } break;
    case eBASETYPE_INT32: {
      pI32 = (int32_t*)pBuf + 1;
      while(len > 1) {
        if(*(int32_t*)pM > *pI32)
          pM = (char*)pI32;
        pI32 ++;
        len --;
      }
    } break;
    case eBASETYPE_UINT32: {
      pU32 = (uint32_t*)pBuf + 1;
      while(len > 1) {
        if(*(uint32_t*)pM > *pU32)
          pM = (char*)pU32;
        pU32 ++;
        len --;
      }
    } break;
    
    case eBASETYPE_FLOAT: {
      pF = (float*)pBuf + 1;
      while(len > 1) {
        if(*(float*)pM > *pF)
          pM = (char*)pF;
        pF ++;
        len --;
      }
    } break;
    
    case eBASETYPE_INT64: {
      pI64 = (int64_t*)pBuf + 1;
      while(len > 1) {
        if(*(int64_t*)pM > *pI64)
          pM = (char*)pI64;
        pI64 ++;
        len --;
      }
    } break;
    case eBASETYPE_UINT64: {
      pU64 = (uint64_t*)pBuf + 1;
      while(len > 1) {
        if(*(uint64_t*)pM > *pU64)
          pM = (char*)pU64;
        pU64 ++;
        len --;
      }
    } break;
    
    case eBASETYPE_DOUBLE: {
      pD = (double*)pBuf + 1;
      while(len > 1) {
        if(*(double*)pM > *pD)
          pM = (char*)pD;
        pD ++;
        len --;
      }
    } break;
  }
  return pM;
}
