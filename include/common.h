#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <time.h>

#include <wiringPi.h>

// target frequency 
#define FREQ 10000000L

// counter pins
#define COUNTER_CS 5
#define COUNTER_READY 6
#define COUNTER_LEN 4

// SPI configs
#define SPI_FREQ 5000000
#define SPI_DEV "/dev/spidev0.0"
#define SPI_MODE SPI_CPHA | SPI_CPOL | SPI_LSB_FIRST | SPI_NO_CS | SPI_READY

// AD7249 pins
#define AD_SCLK 22
#define AD_SDIN 23
#define AD_nSYNC 21

// AD7249 interface configs
// AD7249 t1(min) = 200ns
#define AD_FREQ 5000000
#define AD_DAC 1

// steer configs
/* there are 2 kinds of steer process
   No.1 - fast (archive 10e-8 in 60s, for audiophile uses)
   No.2 - slower but more precise (archive 10e-10 in 27mins, must use double oven OCXO, for instrumental uses)
*/

#define STEER_PROCESS 0
#define STABLE_PIN 4
#define STABLE_LEVEL LOW

const int SteerCounts[2][4] = {
  {10, 5, 6, -1},
  {0, 6, 6, -1}
};

const int SteerStableFlags[2][4] = {
  {-1, 5, -1, -1},
  {-1, -1, -1, 1}
};

#endif
