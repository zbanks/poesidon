#include "project.h"
#include "stdlib.h" // abs

// Nominally 10000
#define ANGLE_GAIN 11000

// Nominally 0
#define ANGLE_ZERO -1000

// Woo magic numbers

int32_t fast_arctan(int32_t y,int32_t x);

int32_t fast_arctan(int32_t y,int32_t x) // returns 10000 * arctan(y/x) for large y/x
{
    return 15708-(7854*x/y - x*((x-y))*(2447 + 0663*x/y)/(y*y));
}

int32_t get_mirror_angle(int32_t y,int32_t x)
{
  if(x>y)
  {
    return 0;
  }

  int32_t a=fast_arctan(y,x)*(ANGLE_GAIN)/7854-(ANGLE_GAIN+ANGLE_ZERO);

  if(a<0)
  {
    return 0;
  }
  if(a>10000)
  {
    return 10000;
  }
  return a;
}

int32_t ramp(int32_t max,int32_t half_period,int32_t time)
{
  return max-abs(max*((time%(half_period*2))-half_period)/half_period);
}

int32_t square(int32_t amp,int32_t half_period,int32_t time)
{
  if(time%(half_period*2)<half_period)
  {
    return -amp;
  }
  return amp;
}

void project(shape_t shape,int depth,int length,int speed,int t)
{
}
