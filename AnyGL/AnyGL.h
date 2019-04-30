#ifndef ANYGL_H
#define ANYGL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "AnyGL_Config.h"

typedef void        (*pfPixel_t)(int16_t x, int16_t y, color_t color);
typedef void        (*pfHVLine_t)(int16_t x, int16_t y, int16_t width, color_t color);

typedef struct {
  pfPixel_t       pfPixel;
  pfHVLine_t      pfHLine;
  pfHVLine_t      pfVLine;

  int16_t         lineWidth;
  int16_t         halfLineWidth;
} sAnyGL_t;

void        AGL_Init(pfPixel_t pfPixel);
void        AGL_SetHVLine(pfHVLine_t pfHLine, pfHVLine_t pfVLine_t);
void        AGL_SetLineWidth(int16_t width);
int16_t     AGL_GetLineWidth(void);
int16_t     AGL_GetHalfLinwWidth(void);

void        AGL_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, color_t color);
void        AGL_HLine(int16_t x, int16_t y, int16_t width, color_t color);
void        AGL_VLine(int16_t x, int16_t y, int16_t height, color_t color);

#ifdef __cplusplus
}
#endif

#endif
