#include "../include/common.h"
#include "../include/ad7249.h"

int dacSetup()
{
  struct timespec holdTime1, holdTime2;
  holdTime1.tv_sec = 0;
  holdTime1.tv_nsec = 50;
  
  // setup GPIO
  pinMode(AD_SCLK, OUTPUT);
  pinMode(AD_SDIN, OUTPUT);
  pinMode(AD_nSYNC, OUTPUT);

  // init state
  digitalWrite(AD_nSYNC, HIGH);
  digitalWrite(AD_SCLK, HIGH);
  digitalWrite(AD_SDIN, LOW);

  // clear both DACs
  dacWriteBinary(1, (uint16_t)0);
  dacWriteBinary(2, (uint16_t)0);

  if (nanosleep(&holdTime1, &holdTime2) < 0)
    return -1;

  return 1;
}

int dacClose()
{
  dacWriteBinary(1, (uint16_t)0);
  dacWriteBinary(1, (uint16_t)0);

  return 1;
}

int dacWriteBinary(int dacNo, uint16_t binary)
{
  int i, j;
  struct timespec cycleTime1, cycleTime2;
  struct timespec holdTime1, holdTime2;

  cycleTime1.tv_sec = 0;
  cycleTime1.tv_nsec = halfCycle;
  holdTime1.tv_sec = 0;
  holdTime1.tv_nsec = 50;

  // select dac
  if (dacNo == 1)
    binary &= ~(1 << DAC_BITS);
  else // dacNo == 2
    binary |= 40970;
    // we don't use dac 2 for now, so use this as a test
    // you should see 1010000000001010 on your logic analyzer

  // 1. set nSYNC
  digitalWrite(AD_nSYNC, LOW);
  if (nanosleep(&holdTime1, &holdTime2) < 0)
    return -1;

  // 2. write 16-bit word
  for (j=0; j<sizeof(uint16_t) * 8; j++)
  {
    i = sizeof(uint16_t) * 8 - j - 1;

    digitalWrite(AD_SCLK, HIGH);
    
    // setup SDIN
    // MSB first - x86 and ARM CPUs are usually little endian
    // attention should be paid, if we port this program to Arduino
    if (binary & (1 << i))
      digitalWrite(AD_SDIN, HIGH);
    else // 0
      digitalWrite(AD_SDIN, LOW);
    if (nanosleep(&cycleTime1, &cycleTime2) < 0)
      return -1;

    digitalWrite(AD_SCLK, LOW);
    if (nanosleep(&cycleTime1, &cycleTime2) < 0)
      return -1;
  }

  // 3. unset nSYNC
  digitalWrite(AD_nSYNC, HIGH);
  digitalWrite(AD_SCLK, HIGH);
  if (nanosleep(&holdTime1, &holdTime2) < 0)
    return -1;

  return 1;
}
