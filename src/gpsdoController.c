#include "../include/common.h"
#include <errno.h>
#include "../include/ad7249.h"
#include "../include/spi.h"

bool exitFlag = false;

void isr(void)
{
  // read 4 bytes from SPI0
  
  // compute freq

  // compute correction

  // control DAC
}

int main(void)
{
  if (wiringPiSetup() < 0)
  {
    fprintf(stderr, "Failed to setup wiringPi: %s\n", strerror(errno));
    return 1;
  }

  if (wiringPiISR(COUNTER_READY, INT_EDGE_RISING, &isr) < 0)
  {
    fprintf(stderr, "Failed to setup ISR: %s\n", strerror(errno));
    return 1;
  }

  // TO DO : switch to native SPI
  // FreqCounter SCLK Fmax > 100MHz
  if (wiringPiSPISetup(0, SPI_FREQ) < 0)
  {
    fprintf(stderr, "Failed to setup SPI: %s\n", strerror(errno));
    return 1;
  }

  if (dacSetup() < 0)
  {
    fprintf(stderr, "Failed to setup DAC\n");
    return 1;
  }

  while(!exitFlag)
  {
    // command system
  }

  return 0;
}
