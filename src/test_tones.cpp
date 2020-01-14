//
// Created by ZodiacTiger on 2019-06-06.
//


#include <cstdlib>
#include <cstdio>
#include "../include/conversions.h"
#include "../include/device.hpp"
#include "../include/test_tones.hpp"
/*************************************************************************************
 *
 *            PRODUCE DUMMY TONE DATA FOR TESTING TX
 *
 *************************************************************************************/


void bladeRF::produce_samples(int16_t *samples, unsigned int num_samples)
{
  static bool data_populated = false;
  unsigned int i;
  samples = (int16_t *) malloc (num_samples * 2 * 1 * sizeof (int16_t));

  if (data_populated) {
    return;
    }

  for (i = 0; i < 2 * num_samples; i += 2) {
    samples[i]     = 2000;
    samples[i + 1] = 2000;
    }
}

void bladeRF::produce_samples(int16_t *samples, unsigned int num_samples,char *output_filename)
{
  static bool data_populated = false;
  unsigned int i;
  samples = (int16_t *) malloc (num_samples * 2 * 1 * sizeof (int16_t));

  FILE *outfile = fopen (output_filename, "w+b");

  if (data_populated) {
    return;
  }

  for (i = 0; i < 2 * num_samples; i += 2) {
    samples[i]     = 2000;
    samples[i + 1] = 2000;
    fwrite (samples, sizeof (*samples) * 2, DEFAULT_BUFFER_SIZE, outfile);
    fflush (outfile);
    }

    fclose(outfile);
}

static void iq_generate_carrier (int16_t *sig_samps, float *fl_sig_samps_out, unsigned int length)
{
  static bool signal_generated = false;
  unsigned int i;

  for (i = 0; i < length * 2; i += 2)
    {
    sig_samps[i] =     6135;
    sig_samps[i + 1] = 2346;
    }
  sc16q11_to_float (sig_samps, fl_sig_samps_out, length);
}