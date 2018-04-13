#include "imuAttitude.h"

sAttitude_t sAtti;
sProcessAtti_t sProAtti;

#ifdef __SUPPORT_INLINE
  #define __STATIC_INLINE       static inline
#else
  #define __STATIC_INLINE
#endif

#define SQUARE(x)       ((x) * (x))

__STATIC_INLINE float lpf1_init(float rate, float cut)
{
  return rate / (rate + 1 / (2 * M_PI * cut));
}

__STATIC_INLINE float lpf1_calc(float old, float newData, float lpf)
{
  return old * (1 - lpf) + newData * lpf;
}

__STATIC_INLINE uint8_t calcOffset(axis3i16_t* axis, axis3f_t* axisSum, uint16_t* count, axis3f_t* offset)
{
  if(count == 0) {
    *count = 1;
    axisSum->x = 0;
    axisSum->y = 0;
    axisSum->z = 0;
    offset->x = 0;
    offset->y = 0;
    offset->z = 0;
    return 0;
  }
  axisSum->x += axis->x;
  axisSum->y += axis->y;
  axisSum->z += axis->z;
  if(*count == 400) {
    count = 0;
    offset->x = axisSum->x / *count;
    offset->y = axisSum->y / *count;
    offset->z = axisSum->z / *count;
    return 1;
  }
  count ++;
  return 0;
}

//原始数据处理变量初始化
void processRawDataInit(sProcessAtti_t* psPA, float lpfSample, float cut, float accel, float sizeA, 
      float gyro, float sizeG)
{
  psPA->accelLsb = (float)((2.0 * accel) / sizeA);
  psPA->gyroLsb = (float)((2.0 * gyro) / sizeG);
  psPA->accel1stlpf = lpf1_init(lpfSample, cut);
}

void processRawDataInitMag(sProcessAtti_t* psPA, float mag, float size)
{
  psPA->magLsb = (float)((2.0 * mag) / size);
}

//原始数据处理
void processRawData(sProcessAtti_t* psPA)
{
  psPA->acc.x -= psPA->accOffSet.x;
  psPA->acc.y -= psPA->accOffSet.y;
  psPA->acc.z -= psPA->accOffSet.z;
  psPA->gyro.x -= psPA->gyroOffset.x;
  psPA->gyro.y -= psPA->gyroOffset.y;
  psPA->gyro.z -= psPA->gyroOffset.z;

  if(psPA->accSumCalc == 0)
    psPA->accSumCalc = calcOffset(&psPA->acc, &psPA->accSum, &psPA->accCount, &psPA->accOffSet);
  if(psPA->gyroSumCalc == 0)
    psPA->gyroSumCalc = calcOffset(&psPA->gyro, &psPA->gyroSum, &psPA->gyroCount, &psPA->gyroOffset);

  psPA->acc.x = lpf1_calc(psPA->accel1stDelay.x, psPA->acc.x, psPA->accel1stlpf);
  psPA->acc.y = lpf1_calc(psPA->accel1stDelay.y, psPA->acc.y, psPA->accel1stlpf);
  psPA->acc.z = lpf1_calc(psPA->accel1stDelay.z, psPA->acc.z, psPA->accel1stlpf);
  psPA->accel1stDelay.x = psPA->acc.x;
  psPA->accel1stDelay.y = psPA->acc.y;
  psPA->accel1stDelay.z = psPA->acc.z;

  psPA->gyro.x = lpf1_calc(psPA->gyro1stDelay.x, psPA->gyro.x, psPA->gyro1stlpf);
  psPA->gyro.y = lpf1_calc(psPA->gyro1stDelay.y, psPA->gyro.y, psPA->gyro1stlpf);
  psPA->gyro.z = lpf1_calc(psPA->gyro1stDelay.z, psPA->gyro.z, psPA->gyro1stlpf);
  psPA->gyro1stDelay.x = psPA->gyro.x;
  psPA->gyro1stDelay.y = psPA->gyro.y;
  psPA->gyro1stDelay.z = psPA->gyro.z;

  psPA->rsltAcc.x = psPA->acc.x * psPA->accelLsb;
  psPA->rsltAcc.y = psPA->acc.y * psPA->accelLsb;
  psPA->rsltAcc.z = psPA->acc.z * psPA->accelLsb;
  psPA->rsltGyro.x = psPA->gyro.x * psPA->gyroLsb;
  psPA->rsltGyro.y = psPA->gyro.y * psPA->gyroLsb;
  psPA->rsltGyro.z = psPA->gyro.z * psPA->gyroLsb;
}

void processRawDataMag(sProcessAtti_t* psPA)
{
//  psPA->rsltMag.x = (float)psPA->mag.x / psPA->magLsb;
//  psPA->rsltMag.y = (float)psPA->mag.y / psPA->magLsb;
//  psPA->rsltMag.z = (float)psPA->mag.z / psPA->magLsb;
  psPA->rsltMag.x = (float)psPA->mag.x / 666.6667;
  psPA->rsltMag.y = (float)psPA->mag.y / 666.6667;
  psPA->rsltMag.z = (float)psPA->mag.z / 666.6667;
}

//设置频率、积分、比例
void imuAttitudeInit(sAttitude_t* psAtti, float rate, float ki, float kp)
{
  psAtti->rate = rate;
  psAtti->ki = ki;
  psAtti->kp = kp;
  psAtti->accZ = 1;
  psAtti->q.q0 = 1.0f;
}

void imuAttitudeInitMag(sAttitude_t* psAtti)
{
  float       roll, pitch;
  float       cosRoll, sinRoll, sinPitch, cosPitch;
  float       magX, magY;
  float       hdg, cosHead, sinHead;

  roll = atan2f(-psAtti->acc.y, -psAtti->acc.z);
  pitch = atan2f(psAtti->acc.x, -psAtti->acc.z);

  cosRoll = cosf(roll);
  sinRoll = sinf(roll);
  cosPitch = cosf(pitch);
  sinPitch = sinf(pitch);

  magX = psAtti->mag.x * cosPitch + psAtti->mag.y * sinRoll * sinPitch + psAtti->mag.z * cosRoll * sinPitch;
  magY = psAtti->mag.y * cosRoll - psAtti->mag.z * sinRoll;

  hdg = atan2f(-magY, magX);

  cosRoll = cosf(roll * 0.5f);
  sinRoll = sinf(roll * 0.5f);

  cosPitch = cosf(pitch * 0.5f);
  sinPitch = sinf(pitch * 0.5f);

  cosHead = cosf(hdg * 0.5f);
  sinHead = sinf(hdg * 0.5f);

  psAtti->q.q0 = cosRoll * cosPitch * cosHead + sinRoll * sinPitch * sinHead;
  psAtti->q.q1 = sinRoll * cosPitch * cosHead - cosRoll * sinPitch * sinHead;
  psAtti->q.q2 = cosRoll * sinPitch * cosHead + sinRoll * cosPitch * sinHead;
  psAtti->q.q3 = cosRoll * cosPitch * sinHead - sinRoll * sinPitch * cosHead;
}

__STATIC_INLINE float invSqrt(float x)
{
  float       halfx = 0.5f * x;
	float       y = x;
	int32_t     i = *(int32_t*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

//imu姿态解算
void imuAttitude(sAttitude_t* psAtti, float* pPitch, float* pRoll, float* pYaw)
{
  float       norm;
  float       ex, ey, ez;
  float       q0s, q1s, q2s, q3s;
  float       halfRate = (1.0f / (float)psAtti->rate) / 2.0;
  float       rate = (1.0f / (float)psAtti->rate);

  //角度转弧度
  psAtti->gyro.x *= DEG2RAD;
  psAtti->gyro.y *= DEG2RAD;
  psAtti->gyro.z *= DEG2RAD;

  if((psAtti->acc.x != 0) || (psAtti->acc.y != 0) || (psAtti->acc.z != 0)) {
    //单位化重力加速度
    norm = invSqrt(SQUARE(psAtti->acc.x) + SQUARE(psAtti->acc.y) + SQUARE(psAtti->acc.z));
    psAtti->acc.x *= norm;
    psAtti->acc.y *= norm;
    psAtti->acc.z *= norm;

    //使用角速度修正偏差
    ex = (psAtti->acc.y * psAtti->mat.partZZ - psAtti->acc.z * psAtti->mat.partYZ);
    ey = (psAtti->acc.z * psAtti->mat.partXZ - psAtti->acc.x * psAtti->mat.partZZ);
    ez = (psAtti->acc.x * psAtti->mat.partYZ - psAtti->acc.y * psAtti->mat.partXZ);

    psAtti->err.x += psAtti->ki * ex * rate;
    psAtti->err.y += psAtti->ki * ey * rate;
    psAtti->err.z += psAtti->ki * ez * rate;

    psAtti->gyro.x += psAtti->kp * ex + psAtti->err.x;
    psAtti->gyro.y += psAtti->kp * ey + psAtti->err.y;
    psAtti->gyro.z += psAtti->kp * ez + psAtti->err.z;
  }

  float       q0l = psAtti->q.q0;
  float       q1l = psAtti->q.q1;
  float       q2l = psAtti->q.q2;
  float       q3l = psAtti->q.q3;

  //近似一阶龙库塔法更新四元数
  psAtti->q.q0 += (-q1l * psAtti->gyro.x - q2l * psAtti->gyro.y - q3l * psAtti->gyro.z) * halfRate;
  psAtti->q.q1 += ( q0l * psAtti->gyro.x + q2l * psAtti->gyro.z - q3l * psAtti->gyro.y) * halfRate;
  psAtti->q.q2 += ( q0l * psAtti->gyro.y - q1l * psAtti->gyro.z + q3l * psAtti->gyro.x) * halfRate;
  psAtti->q.q3 += ( q0l * psAtti->gyro.z + q1l * psAtti->gyro.y - q2l * psAtti->gyro.x) * halfRate;

  //单位化四元数
  norm = invSqrt(SQUARE(psAtti->q.q0) + SQUARE(psAtti->q.q1) + SQUARE(psAtti->q.q2) + SQUARE(psAtti->q.q3));
  psAtti->q.q0 *= norm;
  psAtti->q.q1 *= norm;
  psAtti->q.q2 *= norm;
  psAtti->q.q3 *= norm;
  
//  example quat to eula, one of angle shoud be 89.998901
//  psAtti->q.q0 = 0.707114;
//  psAtti->q.q1 = 0.707100;
//  psAtti->q.q2 = 0;
//  psAtti->q.q3 = 0;

  q0s = SQUARE(psAtti->q.q0);
  q1s = SQUARE(psAtti->q.q1);
  q2s = SQUARE(psAtti->q.q2);
  q3s = SQUARE(psAtti->q.q3);

  //获得矩阵变量值
  psAtti->mat.mat11 = q0s + q1s - q2s - q3s;
  psAtti->mat.mat21 = 2 * (psAtti->q.q1 * psAtti->q.q2 + psAtti->q.q0 * psAtti->q.q3);

  psAtti->mat.partXZ = 2 * (psAtti->q.q1 * psAtti->q.q3 - psAtti->q.q0 * psAtti->q.q2);
  psAtti->mat.partYZ = 2 * (psAtti->q.q0 * psAtti->q.q1 + psAtti->q.q2 * psAtti->q.q3);
  psAtti->mat.partZZ = q0s - q1s - q2s + q3s;

  if(psAtti->mat.partXZ > 1) { psAtti->mat.partXZ = 1; }
  if(psAtti->mat.partXZ < -1) { psAtti->mat.partXZ = -1; }

  //获得欧拉角
  *pPitch = -asinf(psAtti->mat.partXZ) * RAD2DEG;
  *pRoll = atan2f(psAtti->mat.partYZ, psAtti->mat.partZZ) * RAD2DEG;
  *pYaw = atan2f(psAtti->mat.mat21, psAtti->mat.mat11) * RAD2DEG;
}

void imuAttitudeMag(sAttitude_t* psAtti, float* pPitch, float* pRoll, float* pYaw)
{
  float       recipNorm;
  float       halfex = 0.0f, halfey = 0.0f, halfez = 0.0f;
  float       hx, hy, hz, bx, bz;
  float       halfwx, halfwy, halfwz;
  float       halfRate = (1.0f / (float)psAtti->rate) / 2.0;
  float       rate = (1.0f / (float)psAtti->rate);
        
  float       q0q0 = psAtti->q.q0 * psAtti->q.q0;
  float       q0q1 = psAtti->q.q0 * psAtti->q.q1;
  float       q0q2 = psAtti->q.q0 * psAtti->q.q2;
  float       q0q3 = psAtti->q.q0 * psAtti->q.q3;
  float       q1q1 = psAtti->q.q1 * psAtti->q.q0;
  float       q1q2 = psAtti->q.q1 * psAtti->q.q2;
  float       q1q3 = psAtti->q.q1 * psAtti->q.q3;
  float       q2q2 = psAtti->q.q2 * psAtti->q.q2;
  float       q2q3 = psAtti->q.q2 * psAtti->q.q3;
  float       q3q3 = psAtti->q.q3 * psAtti->q.q3; 

  //! If magnetometer measurement is available, use it.
  if(!((psAtti->mag.x == 0.0f) && (psAtti->mag.y == 0.0f) && (psAtti->mag.z == 0.0f))) {

    // Normalise magnetometer measurement
    recipNorm = invSqrt(SQUARE(psAtti->mag.x) + SQUARE(psAtti->mag.y) + SQUARE(psAtti->mag.z));
    psAtti->mag.x *= recipNorm;
    psAtti->mag.y *= recipNorm;
    psAtti->mag.z *= recipNorm;

    // Reference direction of Earth's magnetic field
    hx = 2.0f * (psAtti->mag.x * (0.5f - q2q2 - q3q3) + psAtti->mag.y * (q1q2 - q0q3) + psAtti->mag.z * (q1q3 + q0q2));
    hy = 2.0f * (psAtti->mag.x * (q1q2 + q0q3) + psAtti->mag.y * (0.5f - q1q1 - q3q3) + psAtti->mag.z * (q2q3 - q0q1));
    hz = 2.0f * psAtti->mag.x * (q1q3 - q0q2) + 2.0f * psAtti->mag.y * (q2q3 + q0q1) + 2.0f * psAtti->mag.z * (0.5f - q1q1 - q2q2);
    bx = sqrt(hx * hx + hy * hy);
    bz = hz;

    // Estimated direction of magnetic field
    halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
    halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
    halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex += (psAtti->mag.y * halfwz - psAtti->mag.z * halfwy);
    halfey += (psAtti->mag.z * halfwx - psAtti->mag.x * halfwz);
    halfez += (psAtti->mag.x * halfwy - psAtti->mag.y * halfwx);
  }

  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  if(!((psAtti->acc.x == 0.0f) && (psAtti->acc.y == 0.0f) && (psAtti->acc.z == 0.0f))) {
    float halfvx, halfvy, halfvz;

    // Normalise accelerometer measurement
    recipNorm = invSqrt(SQUARE(psAtti->acc.x) + SQUARE(psAtti->acc.y) + SQUARE(psAtti->acc.z));
    psAtti->acc.x *= recipNorm;
    psAtti->acc.y *= recipNorm;
    psAtti->acc.z *= recipNorm;

    // Estimated direction of gravity and magnetic field
    halfvx = q1q3 - q0q2;
    halfvy = q0q1 + q2q3;
    halfvz = q0q0 - 0.5f + q3q3;

    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex += psAtti->acc.y * halfvz - psAtti->acc.z * halfvy;
    halfey += psAtti->acc.z * halfvx - psAtti->acc.x * halfvz;
    halfez += psAtti->acc.x * halfvy - psAtti->acc.y * halfvx;
  }

  // Apply feedback only when valid data has been gathered from the accelerometer or magnetometer
  if(halfex != 0.0f && halfey != 0.0f && halfez != 0.0f) {
    // Compute and apply integral feedback if enabled
    if(psAtti->ki > 0.0f) {
      psAtti->err.x += psAtti->ki * halfex * rate;	// integral error scaled by Ki
      psAtti->err.y += psAtti->ki * halfey * rate;
      psAtti->err.z += psAtti->ki * halfez * rate;

      // apply integral feedback
      psAtti->gyro.x += psAtti->err.x;
      psAtti->gyro.y += psAtti->err.y;
      psAtti->gyro.z += psAtti->err.z;
    }
    else {
      psAtti->err.x = 0.0f;	// prevent integral windup
      psAtti->err.y = 0.0f;
      psAtti->err.z = 0.0f;
    }

    // Apply proportional feedback
    psAtti->gyro.x += psAtti->kp * halfex;
    psAtti->gyro.y += psAtti->kp * halfey;
    psAtti->gyro.z += psAtti->kp * halfez;
  }

  // Time derivative of quaternion. q_dot = 0.5*q\otimes omega.
  //! q_k = q_{k-1} + dt*\dot{q}
  //! \dot{q} = 0.5*q \otimes P(\omega)
  float dq0 = 0.5f*(-psAtti->q.q1 * psAtti->gyro.x - psAtti->q.q2 * psAtti->gyro.y - psAtti->q.q3 * psAtti->gyro.z);
  float dq1 = 0.5f*( psAtti->q.q0 * psAtti->gyro.x + psAtti->q.q2 * psAtti->gyro.z - psAtti->q.q3 * psAtti->gyro.y);
  float dq2 = 0.5f*( psAtti->q.q0 * psAtti->gyro.y - psAtti->q.q1 * psAtti->gyro.z + psAtti->q.q3 * psAtti->gyro.x);
  float dq3 = 0.5f*( psAtti->q.q0 * psAtti->gyro.z + psAtti->q.q1 * psAtti->gyro.y - psAtti->q.q2 * psAtti->gyro.x); 

  psAtti->q.q0 += rate*dq0;
  psAtti->q.q1 += rate*dq1;
  psAtti->q.q2 += rate*dq2;
  psAtti->q.q3 += rate*dq3;

  // Normalise quaternion
  recipNorm = invSqrt(SQUARE(psAtti->q.q0) + SQUARE(psAtti->q.q1) + SQUARE(psAtti->q.q2) + SQUARE(psAtti->q.q3));
  psAtti->q.q0 *= recipNorm;
  psAtti->q.q1 *= recipNorm;
  psAtti->q.q2 *= recipNorm;
  psAtti->q.q3 *= recipNorm;

  psAtti->mat.mat11 = SQUARE(psAtti->q.q0) + SQUARE(psAtti->q.q1) - SQUARE(psAtti->q.q2) - SQUARE(psAtti->q.q3);
  psAtti->mat.mat21 = 2 * (psAtti->q.q1 * psAtti->q.q2 + psAtti->q.q0 * psAtti->q.q3);

  psAtti->mat.partXZ = 2 * (psAtti->q.q1 * psAtti->q.q3 - psAtti->q.q0 * psAtti->q.q2);
  psAtti->mat.partYZ = 2 * (psAtti->q.q0 * psAtti->q.q1 + psAtti->q.q2 * psAtti->q.q3);
  psAtti->mat.partZZ = SQUARE(psAtti->q.q0) - SQUARE(psAtti->q.q1) - SQUARE(psAtti->q.q2) + SQUARE(psAtti->q.q3);

  if(psAtti->mat.partXZ > 1)
    psAtti->mat.partXZ = 1;
  if(psAtti->mat.partXZ < -1)
    psAtti->mat.partXZ = -1;

  *pPitch = -asinf(psAtti->mat.partXZ) * RAD2DEG;
  *pRoll = atan2f(psAtti->mat.partYZ, psAtti->mat.partZZ) * RAD2DEG;
  *pYaw = atan2f(psAtti->mat.mat21, psAtti->mat.mat11) * RAD2DEG - psAtti->yawOffset;
}

