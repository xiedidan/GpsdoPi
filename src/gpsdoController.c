#include "../include/common.h"
#include <errno.h>
#include "../include/ad7249.h"
#include "../include/spi.h"

bool exitFlag = false;

unsigned char* txBuf;
unsigned char* rxBuf;

void isr(void)
{
  // read 4 bytes from SPI
  digitalWrite(COUNTER_CS, LOW);
  spiTransfer(txBuf, rxBuf, COUNTER_LEN);
  digitalWrite(COUNTER_CS, HIGH);

  // TO DO : compute freq
  uint32_t freq = ();

  // TO DO : compute correction
  uint32_t diff = FREQ - freq;
  uint16_t correction = ();
  
  // control DAC
  dacWrite(correction);
}

int main(void)
{
  txBuf = (unsigned char*)malloc(COUNTER_LEN);
  rxBuf = (unsigned char*)malloc(COUNTER_LEN);

  // GPIO setup
  if (wiringPiSetup() < 0)
  {
    fprintf(stderr, "Failed to setup wiringPi: %s\n", strerror(errno));
    return 1;
  }

  // SPI setup
  // TO DO : verify SPI mode
  // FreqCounter SCLK Fmax > 100MHz
  if (spiSetup(SPI_DEV, SPI_MODE, 8, SPI_FREQ) < 0)
  {
    fprintf(stderr, "Failed to setup SPI: %s\n", strerror(errno));
    return 1;
  }

  // AD7249 setup
  if (dacSetup() < 0)
  {
    fprintf(stderr, "Failed to setup DAC\n");
    return 1;
  }

  // finally, start counter interrupt
  if (wiringPiISR(COUNTER_READY, INT_EDGE_RISING, &isr) < 0)
  {
    fprintf(stderr, "Failed to setup ISR: %s\n", strerror(errno));
    return 1;
  }

  while(!exitFlag)
  {
    // command system
  }

  // release resources
  free(txBuf);
  free(rxBuf);
  spiClose();
  dacClose();

  return 0;
}
