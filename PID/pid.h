#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef struct {
  float   kp;
  float   ki;
  float   kd;

  float   lastErr;
  float   errSum;
  float   setPoint;
} sPID_t;

void    PID_Init(sPID_t *ps, float p, float i, float d, float setPoint);

float   PID_PID(sPID_t *ps, float pv, uint16_t sampleTime);

extern sPID_t   sPID;

#ifdef __cplusplus
}
#endif

#endif
