#ifndef SPI_H
#define SPI_H

int spiSetup(char* spiDevice, uint8_t mode, uint8_t bits, uint32_t speed);
void spiClose();
int spiTransfer(unsigned char* txBuf, unsigned char* rxBuf, int size);
void spiAbort(const char* errStr);

#endif
