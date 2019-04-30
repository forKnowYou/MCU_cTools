#ifndef HC595_H
#define HC595_H

#include "stdint.h"
#include "stdio.h"

typedef void (* HC595_write_t) (uint8_t dat);
typedef void (* HC595_RCK_t) (bool value);
typedef void (* HC595_delayUS_t) (uint16_t t);


class HC595
{
  public:
    HC595(HC595_write_t writeDat, HC595_RCK_t writeRCK, HC595_delayUS_t delayUS);
    
    writeReg(uint8_t dat, bool action);
    
  private:
    HC595_write_t writeDat;
    HC595_RCK_t   writeRCK;
    HC595_delay_t delayUS;
};

#endif
