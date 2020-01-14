//
// Created by ZodiacTiger on 2019-04-26.
//
#include <libbladeRF.h>
#include <iostream>
#include "../include/conversions.h"
#include "../include/channelSetup.hpp"
#include "../include/logging.hpp"
//#include <mm_malloc.h>

  int bladeRF::Config::configure_channel (bladerf *dev, channel_config *c)
  {
    int status;
    uint8_t *buf;
    size_t buf_len;
    const char *filename = "./fw_log.log";

    bladerf_log_set_verbosity (BLADERF_LOG_LEVEL_INFO);

    status = bladerf_get_fw_log (dev, filename);

    status = bladerf_set_frequency (dev, c->channel, c->frequency);
    if (status != 0)
      {
      std::cerr << "Failed to set frequency: " << c->frequency << " -- " <<
                bladerf_strerror (status);
      return status;
      }
    status = bladerf_set_sample_rate (dev, c->channel, c->samplerate, NULL);
    if (status != 0)
      {
      std::cerr << "Failed to set sample rate: " << c->samplerate << " -- " <<
                bladerf_strerror (status);
      return status;
      }
    status = bladerf_set_bandwidth (dev, c->channel, c->bandwidth, NULL);
    if (status != 0)
      {
      std::cerr << "Failed to set bandwidth: " << c->bandwidth << " -- " <<
                bladerf_strerror (status);
      return status;
      }
    status = bladerf_set_gain_mode (dev, c->channel, c->gain_mode);
    if (status != 0)
      {
      std::cerr << "Failed to set gain mode: " << c->frequency << " -- " <<
                bladerf_strerror (status);
      return status;
      }
    status = bladerf_set_gain (dev, c->channel, c->gain);
    if (status != 0)
      {
      std::cerr << "Failed to set gain" << c->gain << " db -- " <<
                bladerf_strerror (status);
      return status;
      }
    status = bladerf_set_bias_tee (dev, c->channel, false);

    return status;
  }

  int bladeRF::Config::enable_module (struct bladerf *dev, bladerf_channel ch, bool enable)
  {
    int status;
    status = bladerf_enable_module (dev, ch, enable);
    if (status != 0)
      {
      std::cout << "Error: Unable to enable module : " << bladerf_strerror (status);

      status = bladerf_enable_module (dev, ch, false);

      return status;
      }
    return status;
  }

int bladeRF::Config::show_device_profile (bladerf *dev, bladerf_devinfo *info, bool show_full_profile)
{
  int status;
  int ret;
  const char *str_chan;
  struct bladerf_version version;
  console_tags tags;

  status = bladerf_fpga_version (dev, &version);
  status = bladerf_get_devinfo (dev, info);
  size_t const rx_ch_count = bladerf_get_channel_count (dev, BLADERF_RX);
  size_t const tx_ch_count = bladerf_get_channel_count (dev, BLADERF_TX);
  char *serial = info->serial;
  struct bladerf_version lib_version;
  bladerf_version (&lib_version);
  bladerf_frequency freq_rx[rx_ch_count];
  for(int i=0;i < rx_ch_count;i++)
    {
      status = bladerf_get_frequency (dev,BLADERF_CHANNEL_RX (i),&freq_rx[i]);
    }
  bladerf_frequency freq_tx[tx_ch_count];
  for(int i=0;i < tx_ch_count;i++)
    {
    status = bladerf_get_frequency (dev,BLADERF_CHANNEL_TX (i),&freq_tx[i]);
    }

  insert_divider (1);

  std::clog << "\n" << info->manufacturer << " (" << info->product << ") SERIAL #" << info->serial;
  std::clog << "\nFPGA Version: " << version.describe;
  std::clog << "\nBladeRF Lib " << lib_version.describe;

  insert_divider (1);
  if (show_full_profile)
    {
    if (strcmp (info->product, "bladeRF 2.0") == 0)
      {
      std::clog << "\n";
      insert_divider (1);
      std::clog << "\n   DEVICE PROFILE [bladeRF 2.0]";
      insert_divider (1);
      status = bladerf_is_fpga_configured (dev);
      if (status != 0)
        {
        std::clog << "\n FPGA " << " " << version.describe
                  << " successfully configured";
        }
      else if (status < 0)
        {
        std::cerr << "\nERROR: Failure to load FPGA";
        }
      if ((rx_ch_count + tx_ch_count) == 4)
        std::clog << "\n    + MIMO Device - 4 Available Channels";
      str_chan = channel2str (BLADERF_CHANNEL_RX(0));
      std::clog << " [ " << str_chan << ",";
      str_chan = channel2str (BLADERF_CHANNEL_RX(1));
      std::clog << str_chan << ",";
      str_chan = channel2str (BLADERF_CHANNEL_TX(0));
      std::clog << str_chan << ",";
      str_chan = channel2str (BLADERF_CHANNEL_TX(1));
      std::clog << str_chan;
      std::clog << " ] ";
      }
    else
      {
      std::clog << "\n";
      insert_divider (1);
      std::clog << "\n   DEVICE PROFILE";
      insert_divider (1);
      std::clog << "\n     2 Available Channels";
      str_chan = channel2str (BLADERF_CHANNEL_RX(0));
      std::clog << " [ " << str_chan << ",";
      str_chan = channel2str (BLADERF_CHANNEL_TX(0));
      std::clog << str_chan;
      std::clog << " ] ";
      }
    }

  std::clog << "\n\n";

  return status;
}


int bladeRF::Config::show_current_status (bladerf *dev, bladerf_channel ch, struct channel_config *c, bladerf_devinfo *info)
{
  int status = 0;

  bladerf_frequency current_freq;

  status = bladerf_get_device_list (&info);
  status = bladerf_get_devinfo (dev, info);
  status = bladerf_get_frequency (dev, ch, &current_freq);
  const char *str_chan;
  size_t const rx_ch_count = bladerf_get_channel_count (dev, BLADERF_RX);
  size_t const tx_ch_count = bladerf_get_channel_count (dev, BLADERF_TX);

  char *usb_bus = info->product;
//  std::string str_backend = bladerf_backend_str (backend);
  insert_divider (1);
  std::clog << "\n   CURRENT CONFIG";
  insert_divider (1);
  uint64_t curr_freq;
  bladerf_frequency freq_rx[rx_ch_count];
  for(int i=0;i < rx_ch_count;i++)
    {
    str_chan = channel2str (BLADERF_CHANNEL_RX(i));
    status = bladerf_get_frequency (dev,BLADERF_CHANNEL_RX (i),&curr_freq);
    freq_rx[i] = curr_freq;
    std::clog << "\n     " << str_chan << " Freq = " << freq_rx[i] << " Hz";
    }
  bladerf_frequency freq_tx[tx_ch_count];
  for(int i=0;i < tx_ch_count;i++)
    {
    str_chan = channel2str (BLADERF_CHANNEL_TX(i));
    status = bladerf_get_frequency (dev,BLADERF_CHANNEL_TX (i),&freq_tx[i]);
    std::clog << "\n     " << str_chan << " Freq = " << freq_rx[i] << " Hz";
    }

  std::clog << "\n\n";

  return status;
}

bladerf_devinfo *bladeRF::Config::getDevInfo ()
{
  return Config::info;
}

void bladeRF::Config::setDevInfo (bladerf_devinfo *infos)
{
  this->info = infos;
}

channel_config *bladeRF::Config::getConfig ()
{
  return Config::c;
}

void bladeRF::Config::setConfig (channel_config *c)
{
  this->c = c;
}

bladerf *bladeRF::Config::getDevice()
{
  return Config::dev;
}

void bladeRF::Config::setDevice (bladerf *dev)
{
  this->dev=dev;
}










