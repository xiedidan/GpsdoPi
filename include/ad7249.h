#ifndef AD7249_H
#define AD7249_H

#define DAC_BITS 12

const int32_t halfCycle = 1000000000L / AD_FREQ / 2;

int dacSetup();
// dacNo = 1 or 2
int dacWriteBinary(int dacNo, uint16_t binrary);
int dacClose();

#endif
