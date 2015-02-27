#include "../include/common.h"
#include <errno.h>
#include "../include/spi.h"

#define T(x) (((x&0xff)<<24)|((x&0xff00)<<8)|((x&0xff0000)>>8)|((x&0xff000000)>>24))

unsigned char* txBuf;
unsigned char* rxBuf;

uint32_t convertFreq(unsigned char* rxBuf);
bool checkFreq(uint32_t freq);

void isr(void)
{
  int i;
  printf("\nisr\n");
  digitalWrite(COUNTER_CS, LOW);
  spiTransfer(txBuf, rxBuf, COUNTER_LEN);
  digitalWrite(COUNTER_CS, HIGH);

  uint32_t freq = convertFreq(rxBuf);
  if (checkFreq(freq))
  // for (i=0; i<4; i++)
    printf("%u", freq);
}

int main(void)
{
  txBuf = (unsigned char*)malloc(COUNTER_LEN);
  rxBuf = (unsigned char*)malloc(COUNTER_LEN);

  if (wiringPiSetup() < 0)
  {
    fprintf(stderr, "Failed to setup wiringPi: %s\n", strerror(errno));
    return 1;
  }
  pinMode(COUNTER_nRST, OUTPUT);
  
  if (spiSetup(SPI_DEV, SPI_MODE, 8, SPI_FREQ) < 0)
  {
    fprintf(stderr, "Failed to setup SPI: %s\n", strerror(errno));
    return 1;
  }

  digitalWrite(COUNTER_nRST, HIGH);
  digitalWrite(COUNTER_nRST, LOW);
  digitalWrite(COUNTER_nRST, HIGH);
  
  if (wiringPiISR(COUNTER_READY, INT_EDGE_RISING, &isr) < 0)
  {
    fprintf(stderr, "Failed to setup ISR: %s\n", strerror(errno));
    return 1;
  }

  while (1)
  {
    // isr();
    // sleep(2);
  }

  return 0;
}

uint32_t convertFreq(unsigned char* rxBuf)
{
  uint32_t freq;

  memcpy(&freq, rxBuf, COUNTER_LEN);
  freq = T(freq);

  return freq;
}

bool checkFreq(uint32_t freq)
{
  if (FREQ * (1 + CHECK_RANGE) < freq || FREQ * (1 - CHECK_RANGE) > freq)
    return false;
  else
    return true;
}
