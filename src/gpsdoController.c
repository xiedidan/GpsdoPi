#include "../include/common.h"
#include <errno.h>
#include "../include/ad7249.h"
#include "../include/spi.h"

bool exitFlag = false;

unsigned char* txBuf;
unsigned char* rxBuf;
uint16_t correction;

uint32_t convertFreq(unsigned char* rxBuf);
void steerOsc(uint32_t freq);

void isr(void)
{
  // read 4 bytes from SPI
  digitalWrite(COUNTER_CS, LOW);
  spiTransfer(txBuf, rxBuf, COUNTER_LEN);
  digitalWrite(COUNTER_CS, HIGH);

  // compute freq
  uint32_t freq = convertFreq(rxBuf);

  // TO DO : steer OCXO
  steerOsc(freq);
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
    // however, this program could never exit
  }

  // release resources
  free(txBuf);
  free(rxBuf);
  spiClose();
  dacClose();

  return 0;
}

uint32_t convertFreq(unsigned char* rxBuf)
{
  uint32_t freq;
  memcpy(&freq, rxBuf, COUNTER_LEN);

  return freq;
}

void steerOsc(uint32_t diff)
{
  // TO DO : steer
}
