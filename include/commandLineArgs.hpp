//
// Created by ZodiacTiger on 2019-05-25.
//

#ifndef BLADERF_EVO_COMMANDLINEARGS_HPP
#define BLADERF_EVO_COMMANDLINEARGS_HPP

#include <getopt.h>
#include <iostream>
#include "logging.hpp"

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"find", optional_argument, 0, 'f'},
    {"make", optional_argument, 0, 'm'},
    {"info", optional_argument, 0, 'i'},
    {"probe", optional_argument, 0, 'p'},

    {"check", optional_argument, 0, 'c'},
    {"sparse", no_argument, 0, 's'},

    {"args", optional_argument, 0, 'a'},
    {"rate", optional_argument, 0, 'r'},
    {"channels", optional_argument, 0, 'n'},
    {"direction", optional_argument, 0, 'd'},
    {0, 0, 0,  0}
};

int long_index = 0;
int option = 0;

static int parseCommandLine(int argc, char *argv[])
{
  std::string argStr;
  std::string chanStr;
  std::string dirStr;
  std::string driverName;

  bool findDevicesFlag(false);
  bool sparsePrintFlag(false);
  bool makeDeviceFlag(false);
  bool probeDeviceFlag(false);

  double sampleRateFlag(0);

  while ((option = getopt_long_only(argc, argv, "", long_options, &long_index)) != -1)
    {
    switch (option)
      {
      case 'h':
        printBanner();
        return printHelp();
      case 'i':
        printBanner();
        return printDeviceInfo ();
      case 'f':
        findDevicesFlag = true;
        if (optarg != nullptr) argStr = optarg;
        break;
      case 'm':
        makeDeviceFlag = true;
        if (optarg != nullptr) argStr = optarg;
        break;
      case 'p':
        probeDeviceFlag = true;
        if (optarg != nullptr) argStr = optarg;
        break;
      case 'c':
        if (optarg != nullptr) driverName = optarg;
        break;
      case 's':
        sparsePrintFlag = true;
        break;
      case 'a':
        if (optarg != nullptr) argStr = optarg;
        break;
      case 'r':
        if (optarg != nullptr) sampleRateFlag = std::stod(optarg);
        break;
      case 'n':
        if (optarg != nullptr) chanStr = optarg;
        break;
      case 'd':
        if (optarg != nullptr) dirStr = optarg;
        break;
      }
    }
  return 0;
}


#endif //BLADERF_EVO_COMMANDLINEARGS_HPP
