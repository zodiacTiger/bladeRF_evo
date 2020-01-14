//
// Created by ZodiacTiger on 2019-05-05.
//




#ifndef BLADERF_EVO_FIRFILTERS_HPP
#define BLADERF_EVO_FIRFILTERS_HPP
#include <cstdlib>
#include <cassert>
#define MAX_INPUT_LEN 80
#define MAX_FLT_LEN 65
#define BUFFER_LEN (MAX_FLT_LEN -1 + MAX_INPUT_LEN)
#define FILTER_LEN  63
#define SAMPLES 80


int16_t coeffs[ FILTER_LEN ] =
    {
        -1468, 1058,   594,   287,    186,  284,   485,   613,
        495,   90,  -435,  -762,   -615,   21,   821,  1269,
        982,    9, -1132, -1721,  -1296,    1,  1445,  2136,
        1570,    0, -1666, -2413,  -1735,   -2,  1770,  2512,
        1770,   -2, -1735, -2413,  -1666,    0,  1570,  2136,
        1445,    1, -1296, -1721,  -1132,    9,   982,  1269,
        821,   21,  -615,  -762,   -435,   90,   495,   613,
        485,  284,   186,   287,    594, 1058, -1468
    };
// Moving average (lowpass) filter of length 8
// There is a null in the spectrum at 1000 Hz
#define FILTER_LEN_MA   8
int16_t coeffsMa[ FILTER_LEN_MA ] =
    {
        32768/8, 32768/8, 32768/8, 32768/8,
        32768/8, 32768/8, 32768/8, 32768/8
    };

int16_t *samples;

class firFilters {

 public:

  int firFilterSetup (int16_t *samples)
  {
    memset (samples, 0, sizeof(samples));
  }

  uint16_t *firNewSamples (uint16_t *in_samples, int length)
  {
    memcpy (&in_samples[MAX_FLT_LEN - 1], in_samples, length * sizeof (int16_t));
    return &in_samples[MAX_FLT_LEN - 1];
  }

  void firShiftSamples (int length)
  {
    memmove (&samples[0], &samples[length], (MAX_FLT_LEN - 1) * sizeof (int16_t));
  }

  void firFixedExec (int16_t *coeffs, int16_t *samples_in, int16_t *samples_out, int length, int filterLength)
  {
    int32_t accum;
    int16_t *coeffp;
    int16_t *in_pnt;
    unsigned int n;
    unsigned int k;

    for (n = 0; n < length; n++)
      {
      coeffp = coeffs;
      in_pnt = &samples_in[n];
      accum = 1 << 14;
      for (k = 0; k < filterLength; k++)
        {
        accum += (int32_t) (*coeffp++) * (int32_t) (*in_pnt--);
        }
      if (accum > 0x3ffffff)
        {
        accum = 0xfffffff;
        }
      else if (accum < -0x40000000)
        {
        accum = -0x40000000;
        }

      samples_out[n] = (int16_t) (accum >> 15);
      }
    }

};

#endif //BLADERF_EVO_FIRFILTERS_HPP
