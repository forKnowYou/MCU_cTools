#ifndef IMUATTITUDE_H
#define IMUATTITUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"

#ifndef M_PI
  #define M_PI        3.14159265358979323846f
#endif

#define DEG2RAD       0.017453293f
#define RAD2DEG       57.29578f

typedef struct {
  float       x;
  float       y;
  float       z;
} axis3f_t;

typedef struct {
  int16_t     x;
  int16_t     y;
  int16_t     z;
} axis3i16_t;

typedef struct {
  float       q0;
  float       q1;
  float       q2;
  float       q3;
} quat4_t;

/*
          cosy*cosk + siny*sink*sina        -cosy*sink + siny*cosk*cosa       -siny*cosa
  Cnb = { sink*cosa                         cosk*cosa                         sina       }
          siny*cosk - cosy*sink*sina        -siny*sink - cosy*cosk*sina       cosy*cosa
          
          T11       T21       T31
          T12       T22       T32
          T13       T23       T33
          
          a(pitch) = arcsin(T32)
          y(roll) = arctan(-T31/T33)
          k(yaw) = arctan(T12/T22)
          
          pitch(y)       mat21     mat11     partXZ
          roll(x)        XXX       XXX       partYZ
          yaw(z)         XXX       XXX       partZZ
*/
typedef struct {
  float       mat11;
  float       mat21;

  float       partXZ;
  float       partYZ;
  float       partZZ;

  float       t11;
  float       t21;
  float       t31;
  float       t12;
  float       t22;
  float       t32;
  float       t13;
  float       t23;
  float       t33;
} matrix_t;

typedef struct {
  axis3f_t      acc;
  axis3f_t      gyro;
  axis3f_t      mag;

  axis3f_t      err;

  quat4_t     q;

  matrix_t    mat;

  float       rate;

  float       ki;
  float       kp;

  float       accZ;
  
  float       yawOffset;
} sAttitude_t;

typedef struct {
  axis3i16_t        acc;
  axis3i16_t        gyro;
  axis3i16_t        mag;

  axis3f_t          rsltAcc;
  axis3f_t          rsltGyro;
  axis3f_t          rsltMag;

  axis3f_t          accSum;
  axis3f_t          accOffSet;
  uint16_t          accCount;
  uint8_t           accSumCalc;

  axis3f_t          gyroSum;
  axis3f_t          gyroOffset;
  uint16_t          gyroCount;
  uint8_t           gyroSumCalc;

  uint8_t           status;

  float       accel1stlpf;
  axis3f_t    accel1stDelay;

  float       gyro1stlpf;
  axis3f_t    gyro1stDelay;
  
  float       accelLsb;
  float       gyroLsb;
  float       magLsb;
} sProcessAtti_t;

void        processRawDataInit(sProcessAtti_t* psPA, float cut, float lpfSample, float accel, float sizeA, 
              float gyro, float sizeB);
void        processRawDataInitMag(sProcessAtti_t* psPA, float mag, float size);
void        processRawData(sProcessAtti_t* psPA);
void        processRawDataMag(sProcessAtti_t* psPA);

void        imuAttitudeInit(sAttitude_t* psAtti, float rate, float ki, float kp);
void        imuAttitudeInitMag(sAttitude_t* psAtti);
void        imuAttitude(sAttitude_t* psAtti, float* pPitch, float* pRoll, float* pYaw);
void        imuAttitudeMag(sAttitude_t* psAtti, float* pPitch, float* pRoll, float* pYaw);

void        imuUpdate(sAttitude_t* psAtti);

extern sAttitude_t sAtti;
extern sProcessAtti_t sProAtti;

#ifdef __cplusplus
}
#endif

#endif
