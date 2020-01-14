//
// Created by ZodiacTiger on 2019-04-28.
//

#ifndef BLADERF_EVO_LOGGING_HPP
#define BLADERF_EVO_LOGGING_HPP

#include <iostream>
#include <libbladeRF.h>

struct console_tags {
  const std::string error = "\033[31m[ ERROR ]\033[0m";
  const std::string info = "\033[32m[ INFO ]\033[0m";
  const std::string warning = "\033[34m[ WARNING ]\033[0m";
  const std::string debug = "\033[36m[ DEBUG ]\033[0m";
  const std::string rx = "\033[33m[  RX  ]\033[0m";
  const std::string tx = "\033[36m[  TX  ]\033[0m";
  const std::string qt = "\033[35m[  QT  ]\033[0m";
};

struct stream_status {
  bladerf_timestamp ts;
  int16_t *sample;
  bool is_running;
};

stream_status
showStreamStats (int16_t *rx_samples, bladerf_metadata meta, bool show_stats, int stat_level, std::chrono::time_point<
    std::chrono::high_resolution_clock> startTime);
void insert_divider (int lines, char *color);
void insert_divider (int lines);
int printBanner ();
int printHelp ();
void printMenuTitle ();
int printDeviceInfo ();

#endif //BLADERF_EVO_LOGGING_HPP
