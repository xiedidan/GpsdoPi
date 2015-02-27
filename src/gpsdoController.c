#include "../include/common.h"
#include <errno.h>
#include "../include/ad7249.h"
#include "../include/spi.h"

bool exitFlag = false;
bool steerFlag = false;
bool skipFlag = false;

unsigned char* txBuf;
unsigned char* rxBuf;

// calibration data
uint32_t maxCount = 0;
uint32_t minCount = 0;
int calibrateTick = 0;

// steer status
int phrase = 0;
int phraseCount = 0;
int tickCount = 0;
uint64_t freqCount = 0;

uint32_t convertFreq(unsigned char* rxBuf);
void calibrate(uint32_t count);
void steer(uint32_t freq);

void isr(void)
{
  // skip the reading right after adjustment (steer, calibration..)
  if (skipFlag)
  {
    skipFlag = false;
    return;
  }

  // read 4 bytes from SPI
  digitalWrite(COUNTER_CS, LOW);
  spiTransfer(txBuf, rxBuf, COUNTER_LEN);
  digitalWrite(COUNTER_CS, HIGH);

  // compute freq
  uint32_t freq = convertFreq(rxBuf);
  if (!checkFreq(freq))
    return;

  if (steerFlag)
  {
    // steer OCXO
    steer(freq);
  }
  else
  {
    // calibration - get steer range
    calibrate(freq);
  }
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
  pinMode(STABLE_PIN, OUTPUT);
  pinMode(COUNTER_nRST, OUTPUT);
  digitalWrite(COUNTER_nRST, HIGH);
  digitalWrite(COUNTER_nRST, LOW);
  digitalWrite(COUNTER_nRST, HIGH);

  // SPI setup
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
  REVERSE(freq);

  return freq;
}

void calibrate(uint32_t count)
{
  if ((calibrateTick > 0) && (calibrateTick <= 5))
  {
    minCount += count;
    if (calibrateTick == 5)
    {
      dacWriteBinary(AD_DAC, (uint16_t)4095);
      minCount /= 5;

      skipFlag = true;
    }
  }
  else if (calibrateTick < 11)
  {
    maxCount += count;
    if (calibrateTick == 10)
    {
      maxCount /= 5;
      steerFlag = true;

      skipFlag = true;
    }
  }

  calibrateTick++;
}

void steer(uint32_t freq)
{
  uint64_t diff = 0;
  uint64_t min = 0;
  uint64_t max = 0;
  double ratio = 0.0;
  uint16_t correction = 0;

  int count = SteerCounts[STEER_PROCESS][phrase];

  if (tickCount < pow(10, phrase))
  {
    freqCount += freq;
    tickCount++;
  }
  else
  {
    // steer
    skipFlag = true;

    diff = FREQ * (tickCount + 1) - freqCount;
    min = minCount * (tickCount + 1);
    max = maxCount * (tickCount + 1);
    if ((freqCount < minCount) || (freqCount > maxCount))
    {
      fprintf(stderr, "%dHz Out of steer range [%d - %d]Hz\n", freqCount, min, max);
      return;
    }

    ratio = diff / (max - min);
    correction = (uint16_t)(ratio * 4096);
    dacWriteBinary(AD_DAC, correction);

    phraseCount++;
    tickCount = 0;
  }

  // stable
  if (SteerStableFlags[STEER_PROCESS][phrase] == phraseCount)
    digitalWrite(STABLE_PIN, STABLE_LEVEL);

  // next phrase
  if ((count != -1) && (phraseCount == count))
  {
    phrase++;
    phraseCount = 0;
  }
}

bool checkFreq(uint32_t freq)
{
  if (FREQ * (1 + CHECK_RANGE) < freq || FREQ * (1 - CHECK_RANGE) > freq)
    return false;
  else
    return true;
}
