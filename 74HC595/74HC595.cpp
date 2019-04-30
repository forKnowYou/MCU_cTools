#include "74HC595.h"

HC595::HC595(HC595_write_t writeDat, HC595_RCK_t writeRCK, HC595_delayUS_t delayUS)
{
  this->writeDat = writeDat;
  this->writeRCK = writeRCK;
  this->delayUS = delayUS;
  this->writeRCK(true);
}


HC595::writeReg(uint8_t dat, bool action)
{
  this->writeRCK(true);
  this->writeDat(dat);
  if(action) {
    this->writeRCK(false);
    delayUS(1);
    this->writeRCK(true);
  }
}

