//
// Created by ZodiacTiger on 2019-05-25.
//

#include <libbladeRF.h>
#include <iostream>
#include "logging.hpp"
#include "device.hpp"
#include "processSamples.hpp"



void insert_divider(const int lines, char *color)
{
  if (strcmp (color, "green") == 0)
  {
    for (unsigned int i = 0; i < lines; i++)
      {
      std::clog << "\n\033[32m---------------------------------------------------------------------\033[0m";
      }
  }
  else if(strcmp (color,"red") == 0)
  {
    for (unsigned int i = 0; i < lines; i++)
      {
      std::clog << "\n\033[31m---------------------------------------------------------------------\033[0m";
      }
  }
  else if(strcmp (color,"yellow") == 0)
    {
    for (unsigned int i = 0; i < lines; i++)
      {
      std::clog << "\n\033[33m---------------------------------------------------------------------\033[0m";
      }
    }
  else if(strcmp (color,"cyan") == 0)
    {
    for (unsigned int i = 0; i < lines; i++)
      {
      std::clog << "\n\033[36m---------------------------------------------------------------------\033[0m";
      }
    }
  else if(strcmp (color,"blue") == 0)
    {
    for (unsigned int i = 0; i < lines; i++)
      {
      std::clog << "\n\033[34m---------------------------------------------------------------------\033[0m";
      }
    }
  else if(strcmp (color,"cyan") == 0)
    {
    for (unsigned int i = 0; i < lines; i++)
      {
      std::clog << "\n\033[36m---------------------------------------------------------------------\033[0m";
      }
    }
}

void insert_divider(const int lines)
{
    for(unsigned int i=0;i < lines;i++)
      {
      std::clog << "\n---------------------------------------------------------------------";
      }
}



int printBanner ()
{
  std::cout << "-----------------------------------------------------" << std::endl;
  std::cout << "|            <<< BladeRF Device Stuff >>>           |" << std::endl;
  std::cout << "-----------------------------------------------------" << std::endl;
  std::cout << std::endl;
  return EXIT_SUCCESS;
}

int printHelp()
{
  std::cout << "Usage SoapySDRUtil [options]" << std::endl;
  std::cout << "  Options summary:" << std::endl;
  std::cout << "    --help \t\t\t\t Print this help message" << std::endl;
  std::cout << "    --info \t\t\t\t Print module information" << std::endl;
  std::cout << "    --find[=\"driver=foo,type=bar\"] \t Discover available devices" << std::endl;
  std::cout << "    --make[=\"driver=foo,type=bar\"] \t Create a device instance" << std::endl;
  std::cout << "    --probe[=\"driver=foo,type=bar\"] \t Print detailed information" << std::endl;
  std::cout << std::endl;

  std::cout << "  Advanced options:" << std::endl;
  std::cout << "    --check[=driverName] \t\t Check if driver is present" << std::endl;
  std::cout << "    --sparse             \t\t Simplified output for --find" << std::endl;
  std::cout << std::endl;

  std::cout << "  Rate testing options:" << std::endl;
  std::cout << "    --args[=\"driver=foo\"] \t\t Arguments for testing" << std::endl;
  std::cout << "    --rate[=stream rate Sps] \t\t Rate in samples per second" << std::endl;
  std::cout << "    --channels[=\"0, 1, 2\"] \t\t List of channels, default 0" << std::endl;
  std::cout << "    --direction[=RX or TX] \t\t Specify the channel direction" << std::endl;
  std::cout << std::endl;
  return EXIT_SUCCESS;
}

int printDeviceInfo ()
{
  std::cout << "-----------------------------------------------------" << std::endl;
  std::cout << "|                 <<< DEVICE INFO >>>               |" << std::endl;
  std::cout << "-----------------------------------------------------" << std::endl;
  std::cout << std::endl;
  return 0;
}

stream_status showStreamStats(int16_t* rx_samples,bladerf_metadata meta, bool show_stats, int stat_level,std::chrono::time_point<std::chrono::high_resolution_clock> startTime)
{
  int status = 0;
  static int overruns = 0;
  console_tags tags;
  auto now = std::chrono::high_resolution_clock::now ();
  static std::chrono::time_point<std::chrono::high_resolution_clock> timeLastStatus;
  stream_status curr_stream;


  if (meta.status & BLADERF_META_STATUS_OVERRUN & show_stats)
    {
    overruns++;
    if(stat_level == 0)
      {
      std::clog << tags.warning
                << tags.rx
                << " Overrun detected ["
                << meta.actual_count << " samples were actually read.]"
                << std::endl;
      }
    else if(stat_level == 1)
      {
      std::clog << tags.warning
                << tags.rx
                << " Overrun detected ["
                << meta.actual_count << " samples were actually read.]"
                << " -- amp=" << getAmp (rx_samples, DEFAULT_BUFFER_SIZE)
                << std::endl;
      }
    }

    if (timeLastStatus + std::chrono::milliseconds(1000) < now)
    {
    timeLastStatus = now;
    const auto timePassed = std::chrono::duration_cast<std::chrono::microseconds> (now - startTime);
    const auto dRate = double (meta.timestamp) / timePassed.count ();
//          sampleCount += meta.actual_count;
//          bytesReceived = sampleCount * (sizeof (*rx_samples) * 2);

    std::clog << tags.rx
              << " [" << meta.actual_count << "/" << DEFAULT_BUFFER_SIZE << "]"
              << " -- ts=" << meta.timestamp
              << " -- Overruns=" << overruns
              << " -- Data Rate=" << bladeRF::device::dRateRound (dRate)<< " ms/s"
              << " -- amp=" << getAmp (rx_samples, DEFAULT_BUFFER_SIZE)
              << " -- phAng=" << getPhaseAng (rx_samples,DEFAULT_BUFFER_SIZE)
              << " deg\n";

    fflush (stdout);
//        window_output = "amp=" + QString::number (getAmp (rx_samples,DEFAULT_BUFFER_SIZE)) + " db";

    }
  return curr_stream;
}
