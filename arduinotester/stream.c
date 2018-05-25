#include "stream.h"

void Stream_Init(sStream_t *psS, sRingBuffer_t *psRBRead, pfWriteBytes_t pfWriteBytes)
{
  psS->readBuf = psRBRead;
  psS->pfWriteBytes = pfWriteBytes;
}

int Stream_Read(sStream_t *psS, uint8_t *pBuf, int len)
{
  return RB_Read(psS->readBuf, pBuf, len);
}

int Stream_Available(sStream_t *psS)
{
  return RB_Available(psS->readBuf);
}

int Stream_ReadLine(sStream_t *psS, uint8_t *pBuf)
{
  return RB_ReadLine(psS->readBuf, pBuf);
}

int Stream_Write(sStream_t *psS, uint8_t *pBuf, int len)
{
  return psS->pfWriteBytes(pBuf, len);
}

int Stream_StreamRead(sStream_t *psS, uint8_t *pBuf, int len)  //read data for peripherals
{
  return RB_Write(psS->readBuf, pBuf, len);
}
