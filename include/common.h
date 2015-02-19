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
#include <wiringPiSPI.h>

// counter pins
#define COUNTER_CS 5
#define COUNTER_READY 6

// SPI speed in Hz
#define SPI_FREQ 5000000
// SPI device
#define SPI_DEV "/dev/spidev0.0"

// AD7249 pins
#define AD_SCLK 1
#define AD_SDIN 2

#endif
