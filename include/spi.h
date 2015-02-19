#ifndef SPI_H
#define SPI_H

int spi;

uint8_t mode;
uint8_t bits = 0;
uint32_t speed = 5000000;
uint16_t delay;

int spiSetup();
void spiClose();
int spiReadBytes(unsigned char* buffer, int size);
int spiWriteBytes(unsigned char* buffer, int size);
void spiAbort(const char* errStr);

#endif
