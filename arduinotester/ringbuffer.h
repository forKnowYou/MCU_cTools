#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "string.h"

typedef struct {
  uint8_t         *pBuf;
  int             len;
  int             head;
  int             available;
} sRingBuffer_t;

void        RB_Init(sRingBuffer_t *psRB, uint8_t *pBuf, int len);
int         RB_Read(sRingBuffer_t *psRB, uint8_t *pBuf, int len);
int         RB_ReadLine(sRingBuffer_t *psRB, uint8_t *pBuf);
int         RB_Write(sRingBuffer_t *psRB, uint8_t *pBuf, int len);
int         RB_Available(sRingBuffer_t *psRB);

#ifdef __cplusplus
}
#endif

#endif
