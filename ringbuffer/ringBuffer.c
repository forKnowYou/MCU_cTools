#include "ringbuffer.h"

void RB_Init(sRingBuffer_t *psRB, uint8_t *pBuf, uint16_t len)
{
  psRB->pBuf = pBuf;
  psRB->len = len;
  psRB->head = 0;
  psRB->available = 0;
}

uint16_t RB_Read(sRingBuffer_t *psRB, uint8_t *pBuf, uint16_t len)
{
  if(! (len > psRB->available)) {
    for(uint16_t i = 0; i < len; i ++) {
      pBuf[i] = psRB->pBuf[(psRB->head + psRB->len - psRB->available) % psRB->len];
      psRB->available --;
    }
    return len;
  }
  return 0;
}

uint8_t RB_ReadOne(sRingBuffer_t *psRB)
{
  uint8_t   temp = 0;
  if(psRB->available) {
    psRB->available --;
    temp = psRB->pBuf[psRB->head];
    psRB->head ++;
    if(psRB->head == psRB->len)
      psRB->head = 0;
  }
  return temp;
}

uint16_t RB_ReadLine(sRingBuffer_t *psRB, uint8_t *pBuf)
{
  uint16_t    available = psRB->available;
  uint16_t    head = psRB->head;
  uint16_t    count = 0;
  while(RB_Available(psRB)) {
    *pBuf = RB_ReadOne(psRB);
    count ++;
    if((*pBuf == '\n') && (*(pBuf - 1) == '\r') && (count > 1)) {
      *(pBuf + 1) = '\0';
      return count;
    }
    pBuf ++;
  }
  psRB->available = available;
  psRB->head = head;
  return 0;
}

uint16_t RB_Write(sRingBuffer_t *psRB, uint8_t *pBuf, uint16_t len)
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

uint16_t RB_Available(sRingBuffer_t *psRB)
{
  return psRB->available;
}
