//
// Created by ZodiacTiger on 2019-06-20.
//

#include <libbladeRF.h>
//#include "device.hpp"
#include "processSamples.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

float getAmp(const int16_t *instream, unsigned int n)
{
  unsigned int i;
  float amplitude=0;
  for (i = 0; i < (2 * n); i += 2)
    {
    amplitude = std::powf ((std::powf((float)instream[i],2) + std::powf((float)instream[i+1],2)), .5);
    }
  return amplitude;
}

float getPhaseAng(const int16_t *instream, unsigned int n)
{
  unsigned int i;
  float phaseAng;
  for (i = 0; i < (2 * n); i += 2)
    {
    phaseAng = std::atan((float)instream[i+1]/(float)instream[i]);
    }
  return phaseAng;
}

void doBasicModulation(const int16_t *instream, int16_t *outstream, unsigned int n, int nn, int t)
{
  unsigned int i;
  unsigned int count=0;
  unsigned int carrier_freq = 100e6;
  for (i = 0; i < (2 * n); i += 2)
    {
    outstream[count] = (float)instream[i] * std::cos (2 * M_1_PI * t * carrier_freq) + (float)instream[i+1] * std::sin (2 * M_1_PI * DEFAULT_BUFFER_SIZE * t * carrier_freq);
    count++;
    }

}