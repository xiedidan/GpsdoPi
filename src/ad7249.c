#include "../include/common.h"
#include "../include/ad7249.h"

int dacSetup()
{
  // setup GPIO
  pinMode(AD_SCLK, OUTPUT);
  pinMode(AD_SDIN, OUTPUT);
  pinMode(AD_nSYNC, OUTPUT);

  // init state
  digitalWrite(AD_nSYNC, HIGH);
  digitalWrite(AD_SCLK, HIGH);
  digitalWrite(AD_SDIN, LOW);

  // clear both DACs
  dacWriteBinrary(1, (uint16_t)0);
  dacWriteBinrary(2, (uint16_t)0);

  return 1;
}

int dacWriteBinrary(int dacNo, uint16_t binrary)
{
  if (dacNo == 1)
    binrary |= 0 << DAC_BITS;
  else // dacNo == 2
    binrary |= 1 << DAC_BITS;

  // 1. set nSYNC
  digitalWrite(AD_SCLK, LOW);
  digitalWrite(AD_nSYNC, LOW);

  // 2. write 16-bit word
  for (int i=0; i<16; i++)
  {
    digitalWrite(AD_SCLK, HIGH);
    // TO DO : setup SDIN

    digitalWrite(AD_SCLK, LOW);
  }

  // 3. unset nSYNC
  digitalWrite(AD_nSYNC, HIGH);
  digitalWrite(AD_SCLK, HIGH);

  return 1;
}
