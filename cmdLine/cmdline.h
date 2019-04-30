#ifndef CMDLINE_H
#define CMDLINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "string.h"
#include "stdlib.h"

#include "ringbuffer.h"

#define CMDL_OK                 0
#define CMDL_ERR_NOPRINT        -1
#define CMDL_ERR_NORINGBUFFER   -2

#define CMDL_READY    ">>>"

typedef int     (*pfCmd_t)(int argc, char **argv);
typedef void*   (*pfBgTask_t)(pfCmd_t pfCmd, int argc, char **argv);
typedef void    (*pfPrint_t)(const char *pCh);

typedef struct {
  const char    *pCmdName;
  pfCmd_t       pfCmd;
} sCmdLine_cmd_t;

typedef struct {
  sCmdLine_cmd_t    *pCmds;
  uint16_t          cmdsLen;

  uint16_t      maxCmdArgvs;
  char          *pMaxCmdArgvsStr;
  char          **ppCmdArgv;

  pfBgTask_t    pfBgTask;
  pfPrint_t     pfPrint;

  sRingBuffer_t   sRingBuffer;
  uint8_t         *pCmdArgvBuf;

  int16_t   isInitSuccessed;
  int16_t   enable;
} sCmdLine_t;

#define CMDL_CREATE_VARIABLES(name, ringBufferLen, maxCmdArgvs) \
  sCmdLine_t    name; \
  uint8_t   CMDL_VAR_ ## name ## _ringBuffer[ringBufferLen] = {0}; \
  uint8_t   CMDL_VAR_ ## name ## _argvsBuffer[ringBufferLen + 1] = {0}; \
  char      *CMDL_VAR_ ## name ## _ppArgvs[maxCmdArgvs] = {0}; \
  char      CMDL_VAR_ ## name ## _pMaxCmdArgvsStr = #maxCmdArgvs

#define CMDL_CONF_VARIABLES(name, cmds) \
  name.pCmds = cmds; \
  name.enable = 1; \
  name.pMaxCmdArgvsStr = CMDL_VAR_ ## name ## _pMaxCmdArgvsStr; \
  name.cmdsLen = sizeof(cmds) / sizeof(sCmdLine_cmd_t); \
  name.maxCmdArgvs = sizeof(CMDL_VAR_ ## name ## _ppArgvs) / sizeof(char **); \
  RB_Init(name.sRingBuffer, CMDL_VAR_ ## name ## _ringBuffer, sizeof(CMDL_VAR_ ## name ## _ringBuffer)); \
  name.pCmdArgvBuf = CMDL_VAR_ ## name ## _ppArgvs

#define CMDL_CONF_FUNCS(name, fPrint, fBgTask) \
  name.pfPrint = fPrint; \
  name.pfBgTask = fBgTask

int16_t   CMDL_Init(sCmdLine_t *psCmdLine);
void      CMDL_Receive(sCmdLine_t *psCmdLine, const char *pRec, uint16_t len);
void      CMDL_Enable(sCmdLine_t *psCmdLine);
void      CMDL_Disable(sCmdLine_t *psCmdLine);

#ifdef __cplusplus
}
#endif

#endif // CMDLINE_H
