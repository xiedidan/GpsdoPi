#include "../include/common.h"
#include "../include/ad7249.h"

int main(void)
{
  uint16_t i = 0;
  struct timespec time1, time2;
  time1.tv_sec = 0;
  time1.tv_nsec = 5000;

  wiringPiSetup();

  dacSetup();
  /*
  for (i=0; i<4096; i++)
  {
    dacWriteBinary(1, i);
    nanosleep(&time1, &time2);
  }
  */
  dacWriteBinary(1, 200);
  // dacClose();

  return 0;
}
