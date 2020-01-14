//
// Created by ZodiacTiger on 2019-06-06.
//

#ifndef BLADERF_EVO_TEST_TONES_HPP
#define BLADERF_EVO_TEST_TONES_HPP

/*************************************************************************************
 *
 *            PRODUCE DUMMY TONE DATA FOR TESTING TX
 *
 *************************************************************************************/

namespace bladeRF
{
  void produce_samples(int16_t *samples, unsigned int num_samples);
  void produce_samples(int16_t *samples, unsigned int num_samples, char *output_filename);
}


#endif //BLADERF_EVO_TEST_TONES_HPP
