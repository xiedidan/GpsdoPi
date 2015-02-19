// TO DO : move headers to common.h
#include "../include/common.h"
#include <linux/spi/spidev.h>

int spiSetup(char* spiDevice, uint8_t mode, uint8_t bits, uint32_t speed)
{
  int ret = 0;
  
  if (spiDevice != NULL)
    ::spiDevice = spiDevice;
  if (mode != NULL)
    ::mode = mode;
  if (bits != NULL)
    ::bits = bits;
  if (speed != NULL)
    ::speed = speed;

  spi = open(device, O_RDWR);
  if (spi < 0)
    spiAbort("Cannot open SPI device");

  // mode
  ret = ioctl(spi, SPI_IOC_WR_MODE, &::mode);
  if (ret == -1)
    spiAbort("Cannot set SPI mode");

  ret = ioctl(spi, SPI_IOC_RD_MODE, &::mode);
  if (ret == -1)
    spiAbort("Cannot get SPI mode");

  // bits
  ret = ioctl(spi, SPI_IOC_WR_BITS_PER_WORD, &::bits);
  if (ret == -1)
    spiAbort("Cannot set bits per word");

  ret = ioctl(spi, SPI_IOC_RD_BITS_PER_WORD, &::bits);
  if (ret == -1)
    spiAbort("Cannot get bits per word");

  // speed
  ret = ioctl(spi, SPI_IOC_WR_MAX_SPEED_HZ, &::speed);
  if (ret == -1)
    spiAbort("Cannot set max speed hz");

  ret = ioctl(spi, SPI_IOC_RD_MAX_SPEED_HZ, &::speed);
  if (ret == -1)
    spiAbort("Cannot get max speed hz");

  return ret;
}

int spiReadBytes(unsigned char* buffer, int size)
{

}

int spiWriteBytes(unsigned char* buffer, int size)
{
}

void spiClose()
{
  close(spi);
}

void spiAbort(const char errStr)
{
  perror(errStr);
  abort();
}

