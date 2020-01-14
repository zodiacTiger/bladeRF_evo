//
// Created by ZodiacTiger on 2019-05-01.
//
#include <stdlib.h>

/* Just a dummy routine to produce a tone at the carrier frequency */
static void produce_samples(int16_t *samples, unsigned int num_samples)
{
  static bool data_populated = false;
  unsigned int i;

  if (data_populated) {
    return;
    }

  for (i = 0; i < 2 * num_samples; i += 2) {
    samples[i]     = 2000;
    samples[i + 1] = 2000;
    }
}
