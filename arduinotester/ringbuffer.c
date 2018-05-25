#include "ringbuffer.h"

void RB_Init(sRingBuffer_t *psRB, uint8_t *pBuf, int len)
{
  psRB->pBuf = pBuf;
  psRB->len = len;
  psRB->head = 0;
  psRB->available = 0;
}

int RB_Read(sRingBuffer_t *psRB, uint8_t *pBuf, int len)
{
  if(! (len > psRB->available)) {
    for(int i = 0; i < len; i ++) {
      pBuf[i] = psRB->pBuf[(psRB->head + psRB->len - psRB->available) % psRB->len];
      psRB->available --;
    }
    return len;
  }
  return 0;
}

int RB_ReadLine(sRingBuffer_t *psRB, uint8_t *pBuf)
{
  int       available = psRB->available;
  int       readLen = RB_Read(psRB, pBuf, available);
  char      *pPos = strstr((const char *) pBuf, "\r\n");
  if(pPos) {
    pPos[2] = '\0';
    int       len = strlen((const char *) pBuf);
    psRB->available += readLen - len;
    return len;
  }
  psRB->available = available;
  return 0;
}

int RB_Write(sRingBuffer_t *psRB, uint8_t *pBuf, int len)
{
  for(int i = 0; i < len; i ++) {
    psRB->pBuf[psRB->head] = pBuf[i];
    psRB->head ++;
    if(psRB->available < psRB->len) {
      psRB->available ++;
    }
    if(psRB->head == psRB->len) {
      psRB->head = 0;
    }
  }
  return len;
}

int RB_Available(sRingBuffer_t *psRB)
{
  return psRB->available;
}
