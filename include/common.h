#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>

#include <wiringPi.h>

// target frequency 
#define FREQ 10000000

// counter pins
#define COUNTER_CS 5
#define COUNTER_READY 6
#define COUNTER_LEN 4

// SPI configs
#define SPI_FREQ 5000000
#define SPI_DEV "/dev/spidev0.0"
#define SPI_MODE SPI_CPHA | SPI_CPOL | SPI_LSB_FIRST | SPI_NO_CS | SPI_READY

// AD7249 pins
#define AD_SCLK 1
#define AD_SDIN 2
#define AD_nSYNC 3

#endif
