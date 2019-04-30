#ifndef ANYGL_CONFIG_H
#define ANYGL_CONFIG_H

#include "stdint.h"

#define COLOR_16        1
#define COLOR_24        0

#if COLOR_16
  #if COLOR_24
    #error can't resolve color type length
  #endif
  typedef int16_t       color_t;
#elif COLOR_24
  typedef struct {
    uint8_t       r;
    uint8_t       g;
    uint8_t       b;
  } color_t;
#else
  #error unknow color type length
#endif

#endif // ANYGL_CONFIG_H
