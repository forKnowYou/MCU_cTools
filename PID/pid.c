#include "pid.h"

sPID_t    sPID;

void PID_Init(sPID_t *ps, float p, float i, float d, float setPoint)
{
  ps->kp = p;
  ps->ki = i;
  ps->kd = d;
  ps->errSum = 0;
  ps->lastErr = 0;
  ps->setPoint = setPoint;
}

// https://en.wikipedia.org/wiki/PID_controller
// out = Kp * e(t) + Ki * (¡Æe(T)dT) + Kd * (d * e(t) / dt)
// where:
//   e(t) = SP - PV, is the error (SP is the setpoint, and PV(t) is the process variable)
//   t is the time or instantaneous time (the present)
//   T is the variable of integration (takes on values from time 0 to the present t).
float PID_PID(sPID_t *ps, float pv, uint16_t sampleTime)
{
  float   err = ps->setPoint - pv;
  ps->errSum += err * sampleTime;
  float   out = ps->kp * err + ps->ki * ps->errSum + ps->kd * ((err - ps->lastErr) / sampleTime);
  ps->lastErr = err;
  return out;
}
