#include "cmdline.h"

int16_t CMDL_Init(sCmdLine_t *psCmdLine)
{
  psCmdLine->isInitSuccessed = 0;
  if(! psCmdLine->cmdsLen)
    return CMDL_OK;
  if(! psCmdLine->sRingBuffer.len)
    return CMDL_ERR_NORINGBUFFER;
  if(! psCmdLine->pfPrint)
    return CMDL_ERR_NOPRINT;
  psCmdLine->isInitSuccessed = 1;
  psCmdLine->sProfile.pfPrint(CMDL_READY);
  return CMDL_OK;
}

void CMDL_Receive(sCmdLine_t *psCmdLine, const char *pRec, uint16_t len)
{
  if(psCmdLine->isInitSuccessed && psCmdLine->enable) {
    RB_Write(&psCmdLine->sRingBuffer, (uint8_t*) pRec, len);
    psCmdLine->pfPrint((char*) pRec);
    if(RB_ReadLine(&psCmdLine->sRingBuffer, psCmdLine->pCmdArgvBuf)) {
      uint16_t    cmdArgvsCount = 0;
      uint16_t    compelete = 0;
      uint16_t    next = 0;
      char        *pCmdArgvBuf = psCmdLine->pCmdArgvBuf;
      while(! compelete) {
        next = 0;
        for(uint16_t i = 0; i < psCmdLine->cmdsLen && ! compelete; i ++) {
          cmdArgvsCount = 0;
          pCmdArgvBuf = strtok(pCmdArgvBuf, " ");
          char        *pArgv = pCmdArgvBuf;
          if(memcmp(pCmdArgvBuf, psCmdLine->pCmds[i].pCmdName, strlen(psCmdLine->pCmds[i].pCmdName)) == 0) {
            pArgv = strtok(pArgv, " ");
            while(pArgv && (cmdArgvsCount < psCmdLine->maxCmdArgvs)) {
              psCmdLine->ppCmdArgv[cmdArgvsCount] = &pArgv;
              if(memcmp(pArgv, "\r\n", 2) == 0) {
                compelete = 1;
                psCmdLine->pCmds[i].pfCmd(psCmdLine->ppCmdArgv, cmdArgvsCount);
                psCmdLine->pfPrint("\r\n");
                psCmdLine->pfPrint(CMDL_READY);
                return;
              }
              if(strstr(pArgv, "&")) {
                if(strstr(pArgv, "&&&")) {
                  psCmdLine->pfPrint("\r\unexpected token \r\n");
                  psCmdLine->pfPrint(CMDL_READY);
                  return;
                } else if(strstr(pArgv, "&&")) {
                  break;
                } else if(strstr(pArgv, "&")) {
                  psCmdLine->pfBgTask(psCmdLine->pCmds[i].pfCmd, psCmdLine->ppCmdArgv, cmdArgvsCount);
                  break;
                }
              }
              if(strstr(pArgv, "|")) {
                if(strstr(pArgv, "|||")) {
                  psCmdLine->pfPrint("\r\unexpected token \r\n");
                  psCmdLine->pfPrint(CMDL_READY);
                  return;
                } else if(strstr(pArgv, "||")) {
                  break;
                } else if(strstr(pArgv, "|") == 0) {
                  psCmdLine->pfPrint("\r\noperate | not support yet \r\n");
                  psCmdLine->pfPrint(CMDL_READY);
                  return;
                }
              }
              cmdArgvsCount ++;
              pArgv = strtok(pArgv, " ");
            }
            if(cmdArgvsCount == psCmdLine->maxCmdArgvs) {
              psCmdLine->pfPrint("\r\ncommand arguments count cant greater than ");
              psCmdLine->pfPrint(psCmdLine->pMaxCmdArgvsStr);
              psCmdLine->pfPrint("\r\n");
              psCmdLine->pfPrint(CMDL_READY);
              return;
            }
          }
        }
        if(! next) {
          psCmdLine->pfPrint("\r\ncommand not found \r\n");
          psCmdLine->pfPrint(CMDL_READY);
          return;
        }
      }

    }
  }
}

void CMDL_Enable(sCmdLine_t *psCmdLine)
{
  psCmdLine->enable = 1;
}

void CMDL_Disable(sCmdLine_t *psCmdLine)
{
  psCmdLine->enable = 0;
}
