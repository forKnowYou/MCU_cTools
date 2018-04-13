#include "MAT_CMD.h"

static void toLowerCase(char* pDat)
{
  while((*pDat != '=') && (*pDat != 0)) {
    if((*pDat >= 'A') && (*pDat <= 'Z'))
      *pDat += 32;
    pDat ++;
  }
}

static inline int checkExe(MAT_OPT_t* pOpt, char* pDat)
{
  if((strncmp(pDat + 2, pOpt->pName, pOpt->nameLen) == 0) && (pDat[pOpt->nameLen + 2] == 0)) {
    if(pOpt->pfExe != 0)
      pOpt->pfExe();
    return 0;
  }
  return -1;
}

static inline int checkQuery(MAT_OPT_t* pOpt, char* pDat)
{
  if((pDat[pOpt->nameLen + 2] == '?') && (pDat[pOpt->nameLen + 3] == 0)) {
    if(pOpt->pfQuery != 0)
      pOpt->pfQuery();
    return 0;
  }
  return -1;
}

static inline int checkTest(MAT_OPT_t* pOpt, char* pDat)
{
  if((pDat[pOpt->nameLen + 2] == '=') && (pDat[pOpt->nameLen + 3] == '?') && (pDat[pOpt->nameLen + 4] == 0)) {
    if(pOpt->pfTest != 0)
      pOpt->pfTest();
    return 0;
  }
  return -1;
}

static inline int checkSet(MAT_OPT_t* pOpt, char* pDat)
{
  if((pDat[pOpt->nameLen + 2] == '=') && (pDat[pOpt->nameLen + 3] > 0)) {
    if(pOpt->pfSet != 0)
      pOpt->pfSet(pDat + pOpt->nameLen + 3);
    return 0;
  }
  return -1;
}

void MAT_init(MAT_CMD_t* pCmd, MAT_OPT_t* pOpt, uint16_t len, pfNull pfError)
{
  uint16_t        cmdCount;
  pCmd->pOpt = pOpt;
  pCmd->len = len;
  pCmd->pfError = pfError;
  while(cmdCount < len) {
    toLowerCase(pCmd->pOpt[cmdCount].pName);
    cmdCount ++;
  }
}

extern void print_a(char* pCh);

void MAT_receive(MAT_CMD_t* pCmd, char* pDat)
{
  uint16_t        cmdCount = 0;
  char*           posEnd;
  toLowerCase(pDat);
  posEnd = strstr(pDat, "\r\n");
  if(posEnd >= 0) {
    posEnd[0] = 0; posEnd[1] = 0;
    print_a(pDat);
    if(strstr(pDat, "at")) {
      while(cmdCount < pCmd->len) {
        if(checkExe(pCmd->pOpt + cmdCount, pDat) == 0)
          break;
        if(checkQuery(pCmd->pOpt + cmdCount, pDat) == 0)
          break;
        if(checkTest(pCmd->pOpt + cmdCount, pDat) == 0)
          break;
        if(checkSet(pCmd->pOpt + cmdCount, pDat) == 0)
          break;
        cmdCount ++;
      }
    }
  }
  if(cmdCount == pCmd->len)
    pCmd->pfError();
  return;
}
