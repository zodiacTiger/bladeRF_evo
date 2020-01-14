//
// Created by ZodiacTiger on 2019-06-14.
//

#ifndef BLADERF_EVO_PROCESSSAMPLES_HPP
#define BLADERF_EVO_PROCESSSAMPLES_HPP

#include <libbladeRF.h>
#include "device.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>

float getAmp(const int16_t *instream, unsigned int n);

float getPhaseAng(const int16_t *instream, unsigned int n);

void doBasicModulation(const int16_t *instream, int16_t *outstream, unsigned int n, int nn, int t);

#endif //BLADERF_EVO_PROCESSSAMPLES_HPP
