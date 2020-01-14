//
// Created by ZodiacTiger on 2019-05-29.
//

#ifndef BLADERF_EVO_DEVICE_HPP
#define BLADERF_EVO_DEVICE_HPP

#include <libbladeRF.h>
#include <cstdio>
#include <logging.hpp>
#include <QtCore>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <complex>

#define DEFAULT_BUFFER_SIZE       (4096)
#define DEFAULT_NUM_BUFFERS        (512)
#define DEFAULT_NUM_TRANSFERS       (32)
#define DEFAULT_TIMEOUT_MS        (3000)
#define DEFAULT_SAMPLERATE     (20000000)
#define DEFAULT_BANDWIDTH      (20000000)
#define DEFAULT_GAIN 44
#define DEFAULT_FREQUENCY 100000000                             // FM
#define DEFAULT_GSM_RX_FREQUENCY 1938800000                     // ARFCN 557
#define DEFAULT_WIFI_RX 2437000000                              // WiFi Channel 6

#define STAT_LEVEL_1 0;
#define STAT_LEVEL_2 1;
#define STAT_LEVEL_3 2;

typedef int stat_level;

struct channel_config {
  bladerf_channel channel;
  unsigned int frequency;
  unsigned int bandwidth;
  unsigned int samplerate;
  int gain;
  bool active;
};

//static bladerf *dev = nullptr;
static bladerf *dev = nullptr;
static bladerf_devinfo d;
static bladerf_devinfo dev_info;
static int16_t *rx_samples = nullptr;
static bool devClosed = false;
static const char *filename = "/tmp/rx_gsm_data.out";
static const char *logfile_name = "bladerf_rxtx_log.txt";
static channel_config config_rx0;
static channel_config config_rx1;
static channel_config config_tx0;
static channel_config config_tx1;
namespace bladeRF
{

class device {
 public:
  device ();
  device (bladerf *dev, const char *devstr);
  ~device ();
  static struct bladerf *getDevice ();
  static int init (bladerf_devinfo *d);
  static int set_iq_balance (std::complex<double> const &balance, bladerf_channel ch);
  static int stream_meta_init_rx (bool mimo);
  static int stream_init_rx (bool mimo);
  static int start_meta_stream_rx (channel_config *c, bool show_stats, int stat_level);
  static int start_meta_stream_rx (channel_config *c, char *output_filename, bool show_stats, int stat_level);
  static int
  qt_start_meta_stream_rx (channel_config *c, char *output_filename, bool show_stats, int stat_level, stream_status curr_stream);
  static int
  qt_thread_start_meta_stream_rx (channel_config *c, char *output_filename, bool show_stats, int stat_level, stream_status curr_stream);
  static int start_stream_rx (channel_config *c);
  static int stream_meta_init_tx (bool mimo);
  static int stream_init_tx (bool mimo);
  static int start_meta_stream_tx (channel_config *c);
  static int start_meta_stream_tx (int16_t *samples, channel_config *c, char *input_filename);
  static int start_stream_tx (channel_config *c);
  static int stream_meta_deinit_rx (int16_t *samples);
  void stream_tx_test_tone ();
  void setDevice (bladerf *dev);
  static void stopStream ();
  static float dRateRound (float var);
  static int close ();
  static int configure_chan (channel_config *c);
  static bool IQbalEnabled ();
  static int show_profile (bool show_full_profile);
  int
  show_profile (bool show_full_profile, channel_config *c1, channel_config *c2, channel_config *c3, channel_config *c4);
  int
  qt_show_profile (bool show_full_profile, channel_config *c1, channel_config *c2, channel_config *c3, channel_config *c4);

  void active_modules ();

 protected:
  void dispose ();

 signals:

 public slots:

 private slots:

};
}
#endif //BLADERF_EVO_DEVICE_HPP
