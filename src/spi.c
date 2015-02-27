#include "../include/common.h"
#include "../include/spi.h"

int spi;

uint8_t spiMode;
uint8_t spiBits = 0;
uint32_t spiSpeed = 5000000;
uint16_t spiDelay;

int spiSetup(char* spiDevice, uint8_t mode, uint8_t bits, uint32_t speed)
{
  int ret = 0;

  // setup GPIO
  pinMode(COUNTER_CS, OUTPUT);
  
  if (spiDevice != NULL)
    spiDevice = spiDevice;
    
  spiMode = mode;
  spiBits = bits;
  spiSpeed = speed;

  spi = open(spiDevice, O_RDWR);
  if (spi < 0)
    spiAbort("Cannot open SPI device");

  // mode
  ret = ioctl(spi, SPI_IOC_WR_MODE, &spiMode);
  if (ret == -1)
    spiAbort("Cannot set SPI mode");

  ret = ioctl(spi, SPI_IOC_RD_MODE, &spiMode);
  if (ret == -1)
    spiAbort("Cannot get SPI mode");

  // bits
  ret = ioctl(spi, SPI_IOC_WR_BITS_PER_WORD, &spiBits);
  if (ret == -1)
    spiAbort("Cannot set bits per word");

  ret = ioctl(spi, SPI_IOC_RD_BITS_PER_WORD, &spiBits);
  if (ret == -1)
    spiAbort("Cannot get bits per word");

  // speed
  ret = ioctl(spi, SPI_IOC_WR_MAX_SPEED_HZ, &spiSpeed);
  if (ret == -1)
    spiAbort("Cannot set max speed hz");

  ret = ioctl(spi, SPI_IOC_RD_MAX_SPEED_HZ, &spiSpeed);
  if (ret == -1)
    spiAbort("Cannot get max speed hz");

  return ret;
}

int spiTransfer(unsigned char* txBuf, unsigned char* rxBuf, int size)
{
  int ret;

  // SPI is full-duplex
  struct spi_ioc_transfer spiTrans;
  spiTrans.tx_buf = (unsigned long)txBuf;
  spiTrans.rx_buf = (unsigned long)rxBuf;
  spiTrans.len = size;
  spiTrans.delay_usecs = spiDelay;
  spiTrans.speed_hz = spiSpeed;
  spiTrans.bits_per_word = spiBits;

  ret = ioctl(spi, SPI_IOC_MESSAGE(1), &spiTrans);
  if (ret < 1)
    spiAbort("Cannot send SPI message");

  return ret;
}

void spiClose()
{
  close(spi);
}

void spiAbort(const char* errStr)
{
  perror(errStr);
  abort();
}

