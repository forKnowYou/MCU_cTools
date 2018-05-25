#include "Arduino.h"

#include "stream.h"

int serialWriteBytes(uint8_t *pBuf, int len)
{
  Serial.println((const char *) pBuf);
  return len;
}

uint8_t       buf[12] = {0};
sRingBuffer_t sRB;
sStream_t     sStream;

void setup()
{
  Serial.begin(115200);

  RB_Init(&sRB, buf, 12);

  Stream_Init(&sStream, &sRB, serialWriteBytes);
}

void loop()
{
  int       available = Serial.available();
  int       lastAvailable = 0;
  while(available != lastAvailable) {
    delay(5);
    lastAvailable = Serial.available();
  }

  if(available) {
    while(Serial.available()) {
      uint8_t       dat = Serial.read();
      Stream_StreamRead(&sStream, &dat, 1);
    }
  }

  if(Stream_Available(&sStream)) {
    uint8_t       buf[12] = {0};
    Serial.println();
    Serial.println(Stream_Available(&sStream));
    int len = Stream_ReadLine(&sStream, buf);
    Serial.println(len);
    Serial.println(Stream_Write(&sStream, buf, len));
  }
}
