#ifndef __STREAM_H
#define __STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#include "ringbuffer.h"

typedef int       (*pfWriteBytes_t)(uint8_t *pBuf, int len);

typedef struct {
  sRingBuffer_t       *readBuf;

  pfWriteBytes_t      pfWriteBytes;
} sStream_t;

void        Stream_Init(sStream_t *psS, sRingBuffer_t *psRBRead, pfWriteBytes_t pfWriteBytes);
int         Stream_Read(sStream_t *psS, uint8_t *pBuf, int len);
int         Stream_Available(sStream_t *psS);
int         Stream_ReadLine(sStream_t *psS, uint8_t *pBuf);
int         Stream_Write(sStream_t *psS, uint8_t *pBuf, int len);

int         Stream_StreamRead(sStream_t *psS, uint8_t *pBuf, int len);  //read data for peripherals

#ifdef __cplusplus
}
#endif

#endif
