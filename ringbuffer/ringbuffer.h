#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "string.h"

typedef struct {
  uint8_t   *pBuf;
  uint16_t  len;
  uint16_t  head;
  uint16_t  available;
} sRingBuffer_t;

void    RB_Init(sRingBuffer_t *psRB, uint8_t *pBuf, uint16_t len);

uint16_t    RB_Read(sRingBuffer_t *psRB, uint8_t *pBuf, uint16_t len);
uint8_t     RB_ReadOne(sRingBuffer_t *psRB);
uint16_t    RB_ReadLine(sRingBuffer_t *psRB, uint8_t *pBuf);
uint16_t    RB_Write(sRingBuffer_t *psRB, uint8_t *pBuf, uint16_t len);
uint16_t    RB_Available(sRingBuffer_t *psRB);

#ifdef __cplusplus
}
#endif

#endif
