#include "AnyGL.h"

sAnyGL_t        gl;

#define SWAP_INT16(a, b) { int16_t c = a; a = b; b = c; }

void AGL_Init(pfPixel_t pfPixel)
{
  gl.pfPixel = pfPixel;
  gl.pfHLine = AGL_HLine;
  gl.pfVLine = AGL_VLine;
  gl.lineWidth = 1;
  gl.halfLineWidth = 0;
}

void AGL_SetHVLine(pfHVLine_t pfHLine, pfHVLine_t pfVLine_t)
{
  gl.pfHLine = pfHLine;
  gl.pfVLine = pfVLine;
}

void AGL_SetLineWidth(int16_t width)
{
  gl.lineWidth = width;
  gl.halfLineWidth = width / 2;
}

int16_t AGL_GetLineWidth()
{
  return gl.lineWidth;
}

int16_t AGL_GetHalfLinwWidth()
{
  return gl.halfLineWidth;
}

void AGL_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, color_t color)
{
  int16_t       diffX = x1 - x2;
  int16_t       diffY = y1 - y2;
  if(diffX < 0)
    diffX = -diffX;
  if(diffY < 0)
    diffY = -diffY;

  if(x1 > x2) {
    SWAP_INT16(x1, x2);
    SWAP_INT16(y1, y2);
  }

  int16_t       scale;
  if(diffX >= diffY) {
    if(diffY == 0) {
      gl.pfHLine(x1, y1, x2 - x1 + 1, color);
    } else {

    }
  } else {

  }
}

void AGL_HLine(int16_t x, int16_t y, int16_t width, color_t color)
{
  for(int16_t i = 0; i < width; i ++) {
    for(int16_t j = 0 - gl.halfLineWidth; j < gl.lineWidth; j ++) {
      gl.pfPixel(x + i, y + j, color);
    }
  }
}

void AGL_VLine(int16_t x, int16_t y, int16_t height, color_t color)
{
  for(int16_t i = 0; i < height; i ++) {
    for(int16_t j = 0 - gl.halfLineWidth; j < gl.lineWidth; j ++) {
      gl.pfPixel(x + j, y + i, color);
    }
  }
}
