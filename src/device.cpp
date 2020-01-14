//
// Created by ZodiacTiger on 2019-05-29.
//
#include <libbladeRF.h>
#include <ctime>
#include <sys/stat.h>
#include <QString>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <boost/thread.hpp>
#include "mainwindow.h"
#include "logging.hpp"
#include "device.hpp"
#include "conversions.h"
#include "processSamples.hpp"
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#include <iomanip>
#include <iostream>
#include <map>

#include <sstream>
#include <string>
#endif

static console_tags tags;
int status = 0;
bool rx_done = false;

void locked_function ()
{
  // function access mutex
  static boost::mutex m;
  // wait for mutex lock
  boost::mutex::scoped_lock lock (m);

  // critical section
  // TODO: Do something

  // auto-unlock on return
}

/*************************************************************************************
 *
 *           DEVICE CONSTRUCTOR & DEVICE OPEN
 *
 *************************************************************************************/

bladeRF::device::device ()
{

  bladerf_init_devinfo (&dev_info);
  status = bladerf_open_with_devinfo (&dev, &dev_info);
  status = bladerf_get_devinfo (dev, &dev_info);
  std::clog << std::endl;

  if (status == 0)
    {
    std::clog << tags.info << " Searching for devices." << std::endl;
    std::clog << tags.info << " " << dev_info.product << " Found." << std::endl;
    }
  else
    {
    std::cout << tags.error << " Boooooooo!! No BladeRF devices found." << std::endl;
//    exit(0);
    }
}

bladeRF::device::~device ()
{
  bladeRF::device::close ();
}

int bladeRF::device::close ()
{
  qDebug () << "\n[  QT  ] Closing Device / GUI\n";
  if (status == 0)
    {
    bladerf_close (dev);
    std::clog << tags.info << " bladeRF Successfully Closed." << std::endl;
    }
  return EXIT_SUCCESS;
}

int bladeRF::device::init (bladerf_devinfo *d)
{
  int status = 0;

  bladerf_init_devinfo (d);
  bladerf_open_with_devinfo (&dev, d);
  bladerf_get_devinfo (dev, d);

  return status;
}

struct bladerf *bladeRF::device::getDevice ()
{
  return dev;
}

void bladeRF::device::setDevice (bladerf *dev)
{
  this->getDevice ();
}

bool bladeRF::device::IQbalEnabled ()
{
  status = bladerf_set_correction (dev, BLADERF_CHANNEL_RX (0), BLADERF_CORR_FPGA_GAIN, 40);
  if (status == 0)
    {
    qDebug () << "IQ Correction Set";
    return true;
    }
  return false;
}

int bladeRF::device::set_iq_balance (std::complex<double> const &balance, bladerf_channel ch)
{
  /* BladeRF IQ correction parameters */
  static const int16_t DCOFF_SCALE = 2048;
  static const int16_t GAIN_SCALE = 4096;
  static const int16_t PHASE_SCALE = 4096;

  int ret = 0;
  int16_t val_gain, val_phase;

  val_gain = static_cast<int16_t>(balance.real () * GAIN_SCALE);
  val_phase = static_cast<int16_t>(balance.imag () * PHASE_SCALE);

  return ret;
}

/*************************************************************************************
 *
 *             Round samplerate to two decimals
 *
 *************************************************************************************/


float bladeRF::device::dRateRound (float var)
{
  // 37.66666 * 100 =3766.66
  // 3766.66 + .5 =37.6716    for rounding off value
  // then type cast to int so value is 3766
  // then divided by 100 so the value converted into 37.66
  float value = (int) (var * 100 + .5);
  return (float) value / 100;
}

/*************************************************************************************
 *
 *             Get Current Time
 *
 *************************************************************************************/

void current_utc_time (struct timespec *ts)
{

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service (mach_host_self (), CALENDAR_CLOCK, &cclock);
  clock_get_time (cclock, &mts);
  mach_port_deallocate (mach_task_self(), cclock);
  ts->tv_sec = mts.tv_sec;
  ts->tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, ts);
#endif

}

/*************************************************************************************
 *
 *             TIME DELAY FUNCTION
 *
 *************************************************************************************/

void time_delay (int milli_seconds)
{
  clock_t start_time = clock ();
  while (clock () < start_time + milli_seconds);
}

/*************************************************************************************
 *
 *           WAIT FOR TIMESTAMP
 *
 *************************************************************************************/

int wait_for_timestamp (struct bladerf *dev,
                        bladerf_direction dir,
                        uint64_t timestamp,
                        unsigned int timeout_ms)
{
  int status;
  uint64_t curr_ts = 0;
  unsigned int slept_ms = 0;
  bool done;

  do
    {
    status = bladerf_get_timestamp (dev, dir, &curr_ts);
    done = (status != 0) || curr_ts >= timestamp;

    if (!done)
      {
      if (slept_ms > timeout_ms)
        {
        done = true;
        status = BLADERF_ERR_TIMEOUT;
        }
      else
        {
        usleep (10000);
        slept_ms += 10;
        }
      }
    }
  while (!done);

  return status;
}

/*************************************************************************************
 *
 *             CONFIGURE CHANNEL
 *
 *************************************************************************************/

int bladeRF::device::configure_chan (channel_config *c)
{
  int status;
  status = bladerf_set_frequency (dev, c->channel, c->frequency);
  if (status != 0)
    {
    fprintf (stderr, "Failed to set frequency = %u: %s\n", c->frequency, bladerf_strerror (status));
    return 1;
    }
  status = bladerf_set_sample_rate (dev, c->channel, c->samplerate, NULL);
  if (status != 0)
    {
    fprintf (stderr, "Failed to set samplerate = %u: %s\n", c->samplerate, bladerf_strerror (status));
    return 1;
    }
  status = bladerf_set_bandwidth (dev, c->channel, c->bandwidth, NULL);
  if (status != 0)
    {
    fprintf (stderr, "Failed to set bandwidth = %u: %s\n", c->bandwidth, bladerf_strerror (status));
    return 1;
    }
  status = bladerf_set_gain (dev, c->channel, c->gain);
  if (status != 0)
    {
    fprintf (stderr, "Failed to set gain: %s\n", bladerf_strerror (status));
    return 1;
    }
//  c->active = true;
  return 0;
}

/*************************************************************************************
 *                           SHOW DEVICE PROPERTIES
 * ***********************************************************************************
 *
 *
 *
 *
 *************************************************************************************/

int
bladeRF::device::show_profile (bool show_full_profile, channel_config *c1, channel_config *c2, channel_config *c3, channel_config *c4)
{
  int status;
  int ret;
  const char *str_chan;
  bladerf_devinfo dev_info;
  struct bladerf_version version;
  const struct bladerf_range *gain_range;
  const struct bladerf_range *freq_range;
  const struct bladerf_range *samplerate_range;
  const struct bladerf_range *bandwidth_range;
  const char *fpga_name = "none";
  bladerf_fpga_size *fpgaSize;
  bladerf_get_fpga_size (dev, fpgaSize);
  bladerf_backend *backend;
  struct timespec ts;
  current_utc_time (&ts);
  status = bladerf_fpga_version (dev, &version);
  size_t const rx_ch_count = bladerf_get_channel_count (dev, BLADERF_RX);
  size_t const tx_ch_count = bladerf_get_channel_count (dev, BLADERF_TX);
  ret = bladerf_get_gain_range (dev, BLADERF_CHANNEL_RX(0), &gain_range);
  ret = bladerf_get_frequency_range (dev, BLADERF_CHANNEL_RX (0), &freq_range);
  ret = bladerf_get_sample_rate_range (dev, BLADERF_CHANNEL_RX (0), &samplerate_range);
  ret = bladerf_get_bandwidth_range (dev, BLADERF_CHANNEL_RX (0), &bandwidth_range);
  char *serial = dev_info.serial;
  struct bladerf_version lib_version;
  bladerf_version (&lib_version);
  insert_divider (1, "green");

  std::clog << "\n" << dev_info.manufacturer << " (" << dev_info.product << ") SERIAL #" << serial;
  std::clog << "\nFPGA Version: " << version.describe;
  std::clog << "\nBladeRF Lib " << lib_version.describe;
  insert_divider (1, "green");
  if (show_full_profile)
    {
    if (strcmp (dev_info.product, "bladeRF 2.0") == 0)
      {
      std::clog << "\n";
      insert_divider (1);
      std::clog << "\n                      DEVICE  PROFILE";
      insert_divider (1);
      status = bladerf_is_fpga_configured (dev);
      backend = &dev_info.backend;
      if (status != 0)
        {
        std::clog << "\n  Current device instance ID=" << dev_info.instance
                  << ", USB Backend=" << bladerf_backend_str (*backend) << std::endl;
        std::clog << "\n  FPGA " << "[" << version.describe
                  << "] configured" << std::endl;
        }
      else if (status < 0)
        {
        std::cerr << "\nERROR: Failure to load FPGA";
        }
      if ((rx_ch_count + tx_ch_count) > 3)
        {
        std::clog << "\n  MIMO Capable Device - " << rx_ch_count + tx_ch_count << " Available Channels.\n";
        }
      std::clog << "    + RX x " << rx_ch_count << " [";
      if (c1->active)
        {
        std::clog << channel2str (BLADERF_CHANNEL_RX (0)) << "(active)|";
        }
      else
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_RX(0)) <<
                  "|";
        }

      if (c2->active)
        {
        std::clog << channel2str (BLADERF_CHANNEL_RX(1)) << "(active)]\n";
        }
      else
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_RX(1)) << "]\n";
        }

      std::clog << "    + TX x " << tx_ch_count << " [";

      if (c3->active)
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_TX(0)) << "(active)|";
        }
      else
        {
        std::clog << channel2str (BLADERF_CHANNEL_TX (0)) << "|";
        }

      if (c4->active)
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_TX(1)) << "(active)" <<
                  "]\n";
        }
      else
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_TX(1)) <<
                  "]\n";
        }
      }
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

  ret = bladerf_get_sample_rate_range (dev, BLADERF_CHANNEL_RX (0), &samplerate_range);
  ret = bladerf_get_bandwidth_range (dev, BLADERF_CHANNEL_RX (0), &bandwidth_range);

  std::clog << "\n  Available samplerates [" << samplerate_range->min << "-" << samplerate_range->max << " Ms/s]"
            << std::endl;
  std::clog << "  Available bandwidths  [" << bandwidth_range->min << "-" << bandwidth_range->max << " Ms/s]"
            << std::endl;

  std::clog << "\n  Frequency and gain ranges" << std::endl;

  for (unsigned int i = 0; i < rx_ch_count; i++)
    {
    ret = bladerf_get_frequency_range (dev, BLADERF_CHANNEL_RX (i), &freq_range);
    ret = bladerf_get_gain_range (dev, BLADERF_CHANNEL_RX(i), &gain_range);
    std::clog << "    + RX Channel " << (i + 1) << " [" << freq_range->min << "-" << freq_range->max << " Hz] , ["
              << gain_range->min << "-" << gain_range->max << " db]" << std::endl;

    }
  for (unsigned int i = 0; i < tx_ch_count; i++)
    {
    ret = bladerf_get_frequency_range (dev, BLADERF_CHANNEL_TX (i), &freq_range);
    ret = bladerf_get_gain_range (dev, BLADERF_CHANNEL_TX(i), &gain_range);

    std::clog << "    + TX Channel " << (i + 1) << " [" << freq_range->min << "-" << freq_range->max << " Hz] , ["
              << gain_range->min << "-" << gain_range->max << " db]" << std::endl;
    }

  std::clog << "\n\n";
  return 0;
}

int
bladeRF::device::qt_show_profile (bool show_full_profile, channel_config *c1, channel_config *c2, channel_config *c3, channel_config *c4)
{
  int status;
  int ret;
  const char *str_chan;
  bladerf_devinfo dev_info;
  struct bladerf_version version;
  const struct bladerf_range *gain_range;
  const struct bladerf_range *freq_range;
  const struct bladerf_range *samplerate_range;
  const struct bladerf_range *bandwidth_range;

  bladerf_backend *backend;
  struct timespec ts;
  current_utc_time (&ts);
  status = bladerf_fpga_version (dev, &version);

  size_t const rx_ch_count = bladerf_get_channel_count (dev, BLADERF_RX);
  size_t const tx_ch_count = bladerf_get_channel_count (dev, BLADERF_TX);
  ret = bladerf_get_gain_range (dev, BLADERF_CHANNEL_RX(0), &gain_range);
  ret = bladerf_get_frequency_range (dev, BLADERF_CHANNEL_RX (0), &freq_range);
  ret = bladerf_get_sample_rate_range (dev, BLADERF_CHANNEL_RX (0), &samplerate_range);
  ret = bladerf_get_bandwidth_range (dev, BLADERF_CHANNEL_RX (0), &bandwidth_range);
  char *serial = dev_info.serial;
  struct bladerf_version lib_version;
  bladerf_version (&lib_version);
  insert_divider (1, "green");

  std::clog << "\n" << dev_info.manufacturer << " (" << dev_info.product << ") SERIAL #" << serial;
  std::clog << "\nFPGA Version: " << version.describe;

  std::clog << "\nBladeRF Lib " << lib_version.describe;
  insert_divider (1, "green");
  if (show_full_profile)
    {
    if (strcmp (dev_info.product, "bladeRF 2.0") == 0)
      {
      std::clog << "\n";
      insert_divider (1);
      std::clog << "\n                      DEVICE  PROFILE";
      insert_divider (1);
      status = bladerf_is_fpga_configured (dev);
      backend = &dev_info.backend;
      if (status != 0)
        {
        std::clog << "\n  Current device instance ID=" << dev_info.instance
                  << ", USB Backend=" << bladerf_backend_str (*backend) << std::endl;
        std::clog << "\n  FPGA " << "[" << version.describe
                  << "] configured" << std::endl;
        }
      else if (status < 0)
        {
        std::cerr << "\nERROR: Failure to load FPGA";
        }
      if ((rx_ch_count + tx_ch_count) > 3)
        {
        std::clog << "\n  MIMO Capable Device - " << rx_ch_count + tx_ch_count << " Available Channels.\n";
        }
      std::clog << "    + RX x " << rx_ch_count << " [";
      if (c1->active)
        {
        std::clog << channel2str (BLADERF_CHANNEL_RX (0)) << "(active)|";
        }
      else
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_RX(0)) <<
                  "|";
        }

      if (c2->active)
        {
        std::clog << channel2str (BLADERF_CHANNEL_RX(1)) << "(active)]\n";
        }
      else
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_RX(1)) << "]\n";
        }

      std::clog << "    + TX x " << tx_ch_count << " [";

      if (c3->active)
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_TX(0)) << "(active)|";
        }
      else
        {
        std::clog << channel2str (BLADERF_CHANNEL_TX (0)) << "|";
        }

      if (c4->active)
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_TX(1)) << "(active)" <<
                  "]\n";
        }
      else
        {
        std::clog <<
                  channel2str (BLADERF_CHANNEL_TX(1)) <<
                  "]\n";
        }
      }
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
  char *test_serial;

  ret = bladerf_get_sample_rate_range (dev, BLADERF_CHANNEL_RX (0), &samplerate_range);
  ret = bladerf_get_bandwidth_range (dev, BLADERF_CHANNEL_RX (0), &bandwidth_range);

  std::clog << "\n  Available samplerates [" << samplerate_range->min << "-" << samplerate_range->max << " Ms/s]"
            << std::endl;
  std::clog << "  Available bandwidths  [" << bandwidth_range->min << "-" << bandwidth_range->max << " Ms/s]"
            << std::endl;

  std::clog << "\n  Frequency and gain ranges" << std::endl;

  for (unsigned int i = 0; i < rx_ch_count; i++)
    {
    ret = bladerf_get_frequency_range (dev, BLADERF_CHANNEL_RX (i), &freq_range);
    ret = bladerf_get_gain_range (dev, BLADERF_CHANNEL_RX(i), &gain_range);
    std::clog << "    + RX Channel " << (i + 1) << " [" << freq_range->min << "-" << freq_range->max << " Hz] , ["
              << gain_range->min << "-" << gain_range->max << " db]" << std::endl;

    }
  for (unsigned int i = 0; i < tx_ch_count; i++)
    {
    ret = bladerf_get_frequency_range (dev, BLADERF_CHANNEL_TX (i), &freq_range);
    ret = bladerf_get_gain_range (dev, BLADERF_CHANNEL_TX(i), &gain_range);

    std::clog << "    + TX Channel " << (i + 1) << " [" << freq_range->min << "-" << freq_range->max << " Hz] , ["
              << gain_range->min << "-" << gain_range->max << " db]" << std::endl;
    }

  std::clog << "\n\n";
  return 0;
}

/*************************************************************************************
 *
 *             INITIALIZE META RX STREAM
 *
 *************************************************************************************/

int bladeRF::device::stream_meta_init_rx (bool mimo)
{
  const unsigned int num_buffers = DEFAULT_NUM_BUFFERS;
  const unsigned int buffer_size = DEFAULT_BUFFER_SIZE; /* Must be a multiple of 1024 */
  const unsigned int num_transfers = DEFAULT_NUM_TRANSFERS;
  const unsigned int timeout_ms = DEFAULT_TIMEOUT_MS;

  status = bladerf_sync_config (dev, BLADERF_RX_X1, BLADERF_FORMAT_SC16_Q11_META, num_buffers, buffer_size, num_transfers, timeout_ms);
//  status = bladerf_enable_module (dev, BLADERF_RX, true);


  FILE *log = fopen (logfile_name, "a");
  FILE *outfile = fopen (filename, "w+b");

  if (status != 0)
    {
    fprintf (stderr, "Failed to configure RX interface: %s\n",
             bladerf_strerror (status));
    return status;
    }
  else
    {
    std::clog << tags.info << " Meta RX Stream Initialized." << std::endl;
    }

  return status;
}

/*************************************************************************************
 *
 *             INITIALIZE NON-META RX STREAM
 *
 *************************************************************************************/

int bladeRF::device::stream_init_rx (bool mimo)
{
  const unsigned int num_buffers = DEFAULT_NUM_BUFFERS;
  const unsigned int buffer_size = DEFAULT_BUFFER_SIZE; /* Must be a multiple of 1024 */
  const unsigned int num_transfers = DEFAULT_NUM_TRANSFERS;
  const unsigned int timeout_ms = DEFAULT_TIMEOUT_MS;

  status = bladerf_sync_config (dev, BLADERF_RX_X1, BLADERF_FORMAT_SC16_Q11, num_buffers, buffer_size, num_transfers, timeout_ms);

  if (status != 0)
    {
    fprintf (stderr, "Failed to configure RX interface: %s\n", bladerf_strerror (status));
    return status;
    }
  else
    {
    std::clog << tags.info << " non-meta RX Stream Initialized." << std::endl;
    }
  return status;
}

/*************************************************************************************
 *
 *             START META RX STREAM
 *
 *************************************************************************************/


int bladeRF::device::start_meta_stream_rx (channel_config *c, bool show_stats, int stat_level)
{
  bool rx_done = false;
  int16_t *rx_samples = NULL;
  int16_t *rx_samples_out = NULL;
  const unsigned int rx_len = 20000;
  console_tags tags;
  unsigned int i;
  struct bladerf_metadata meta;
  long sampleCount (0);
  long bytesReceived (0);
  int overruns (0);
  time_t curr_t;
  char *current_time = ctime (&curr_t);
  FILE *log = fopen (logfile_name, "a");
  fprintf (log, "RXTX ran @ %s", current_time);
  auto startTime = std::chrono::high_resolution_clock::now ();
  auto timeLimit = std::chrono::high_resolution_clock::now ();
  auto timeLastStatus = std::chrono::high_resolution_clock::now ();
  memset (&meta, 0, sizeof (meta));
  meta.flags = BLADERF_META_FLAG_RX_NOW;
  rx_samples = (int16_t *) malloc (rx_len * 2 * 1 * sizeof (int16_t));

  rx_samples_out = (int16_t *) malloc (rx_len * 2 * 1 * sizeof (int16_t));

  if (rx_samples == nullptr)
    {
    perror ("malloc");
    return BLADERF_ERR_MEM;
    }
  if (status != 0)
    {
    perror ("Unable To Initialize BladeRF Meta Stream");
    return BLADERF_ERR_NOT_INIT;
    }
  status = bladerf_enable_module (dev, c->channel, true);
  if (status != 0)
    {
    std::cerr << "Failed to enable RX: " << bladerf_strerror (status) << std::endl;
    }
  float amplitude = 0;

  while (!rx_done && status == 0)
    {
    const auto now = std::chrono::high_resolution_clock::now ();
    status = bladerf_sync_rx (dev, rx_samples, DEFAULT_BUFFER_SIZE, &meta, DEFAULT_TIMEOUT_MS);
    if (status != 0)
      {
      std::cerr << tags.error << tags.rx << "Failed to start stream: " <<
                bladerf_strerror (status)
                << std::endl;
      }
    else if (meta.status & BLADERF_META_STATUS_OVERRUN & show_stats)
      {
      overruns++;
      if (stat_level == 0)
        {
        std::clog << tags.warning
                  << tags.rx
                  << " Overrun detected ["
                  << meta.actual_count << " samples were actually read.]"
                  << std::endl;
        }
      else if (stat_level == 1)
        {
        std::clog << tags.warning
                  << tags.rx
                  << " Overrun detected ["
                  << meta.actual_count << " samples were actually read.]"
                  << " -- amp=" << getAmp (rx_samples, DEFAULT_BUFFER_SIZE)
                  << std::endl;
        }
      }
    else
      {
      if (timeLastStatus + std::chrono::seconds (1) < now)
        {
        timeLastStatus = now;
        const auto timePassed = std::chrono::duration_cast<std::chrono::microseconds> (now - startTime);
        const auto dRate = double (meta.timestamp) / timePassed.count ();
//          sampleCount += meta.actual_count;
//          bytesReceived = sampleCount * (sizeof (*rx_samples) * 2);

        std::clog << tags.rx
                  << " [" << meta.actual_count << "/" << DEFAULT_BUFFER_SIZE << "] Samples RX'd --"
                  << " ts=" << meta.timestamp << " -- Overruns=" << overruns << " -- Data Rate=" << dRateRound (dRate)
                  << " ms/s"
                  << " -- amp=" << getAmp (rx_samples, DEFAULT_BUFFER_SIZE) << " db"
                  << " -- phAng=" << getPhaseAng (rx_samples, DEFAULT_BUFFER_SIZE) << " deg"
                  << std::endl;

        doBasicModulation (rx_samples, rx_samples_out, DEFAULT_BUFFER_SIZE, 4, 5);
        std::clog << tags.rx
                  << " [" << meta.actual_count << "/" << DEFAULT_BUFFER_SIZE << "] Samples RX'd --"
                  << " ts=" << meta.timestamp << " -- Overruns=" << overruns << " -- Data Rate=" << dRateRound (dRate)
                  << " ms/s"
                  << " -- amp=" << getAmp (rx_samples_out, DEFAULT_BUFFER_SIZE) << " db"
                  << " -- phAng=" << getPhaseAng (rx_samples_out, DEFAULT_BUFFER_SIZE) << " deg"
                  << std::endl;

        fflush (stdout);
        }

      }
    }
  return 0;
}

/*************************************************************************************
 *
 *             START META RX STREAM
 *
 *************************************************************************************/
/**
 * Peform adjustments on received samples before writing them out:
 *  (1) Mask off FPGA markers
 *  (2) Convert little-endian samples to host endianness, if needed.
 *
 *  @param  buff    Sample buffer
 *  @param  n       Number of samples
 */
int bladeRF::device::start_meta_stream_rx (channel_config *c, char *output_filename, bool show_stats, int stat_level)
{
  bool rx_done = false;
  int16_t *rx_samples = NULL;
  int16_t *rx_samples_out = NULL;

  const unsigned int rx_len = 20000;
  console_tags tags;
  unsigned int i;
  struct bladerf_metadata meta;
  long sampleCount (0);
  long bytesReceived (0);
  int overruns (0);
  time_t curr_t;
  char *current_time = ctime (&curr_t);

  FILE *log = fopen (logfile_name, "a");


//  FILE *outfile = stdout;

  FILE *outfile = fopen (output_filename, "w+b");

  fprintf (log, "RXTX ran @ %s", current_time);
  auto startTime = std::chrono::high_resolution_clock::now ();
  auto timeLimit = std::chrono::high_resolution_clock::now ();
  auto timeLastStatus = std::chrono::high_resolution_clock::now ();
  memset (&meta, 0, sizeof (meta));
  meta.flags = BLADERF_META_FLAG_RX_NOW;
  rx_samples = (int16_t *) malloc (DEFAULT_BUFFER_SIZE * 2 * sizeof (int16_t));
  rx_samples_out = (int16_t *) malloc (DEFAULT_BUFFER_SIZE * 2 * sizeof (int16_t));

  if (rx_samples == nullptr)
    {
    perror ("malloc");
    return BLADERF_ERR_MEM;
    }
  if (status != 0)
    {
    perror ("Unable To Initialize BladeRF Meta Stream");
    return BLADERF_ERR_NOT_INIT;
    }
  status = bladerf_enable_module (dev, c->channel, true);
  if (status != 0)
    {
    std::cerr << "Failed to enable RX: " << bladerf_strerror (status) << std::endl;
    }

  while (!rx_done && status == 0)
    {
    const auto now = std::chrono::high_resolution_clock::now ();
    status = bladerf_sync_rx (dev, rx_samples, DEFAULT_BUFFER_SIZE, &meta, DEFAULT_TIMEOUT_MS);
//    doBasicModulation (rx_samples,rx_samples_out,DEFAULT_BUFFER_SIZE,1,5);

//    fwrite (rx_samples_out, sizeof (*rx_samples_out), DEFAULT_BUFFER_SIZE, outfile);
    fwrite (rx_samples, sizeof (*rx_samples) * 2, DEFAULT_BUFFER_SIZE, outfile);
    fflush (outfile);
    if (status != 0)
      {
      std::cerr << tags.error << tags.rx << "Failed to start stream: " <<
                bladerf_strerror (status)
                << std::endl;
      }
    else if (meta.status & BLADERF_META_STATUS_OVERRUN & show_stats)
      {
      overruns++;
      if (stat_level == 0)
        {
        std::clog << tags.warning
                  << tags.rx
                  << " Overrun detected ["
                  << meta.actual_count << " samples were actually read.]"
                  << std::endl;
        }
      else if (stat_level == 1)
        {
        std::clog << tags.warning
                  << tags.rx
                  << " Overrun detected ["
                  << meta.actual_count << " samples were actually read.]"
                  << " -- amp=" << getAmp (rx_samples, DEFAULT_BUFFER_SIZE)
                  << std::endl;
        }
      }
    else
      {
      if (timeLastStatus + std::chrono::seconds (1) < now)
        {
        timeLastStatus = now;
        const auto timePassed = std::chrono::duration_cast<std::chrono::microseconds> (now - startTime);
        const auto dRate = double (meta.timestamp) / timePassed.count ();
//          sampleCount += meta.actual_count;
//          bytesReceived = sampleCount * (sizeof (*rx_samples) * 2);

        std::clog << tags.rx
                  << " [" << meta.actual_count << "/" << DEFAULT_BUFFER_SIZE << "] Samples RX'd --"
                  << " ts=" << meta.timestamp << " -- Overruns=" << overruns << " -- Data Rate=" << dRateRound (dRate)
                  << " ms/s"
                  << " -- amp=" << getAmp (rx_samples, DEFAULT_BUFFER_SIZE) << " db"
                  << " -- phAng=" << getPhaseAng (rx_samples, DEFAULT_BUFFER_SIZE) << " deg"
                  << std::endl;
        fflush (stdout);
        }
      }
    }
  fclose (outfile);
  return 0;
}

/*************************************************************************************
 *
 *         QT    START META RX STREAM
 *
 *************************************************************************************/
/**
 * Peform adjustments on received samples before writing them out:
 *  (1) Mask off FPGA markers
 *  (2) Convert little-endian samples to host endianness, if needed.
 *
 *  @param  buff    Sample buffer
 *  @param  n       Number of samples
 */
int
bladeRF::device::qt_start_meta_stream_rx (channel_config *c, char *output_filename, bool show_stats, int stat_level, stream_status curr_stream)
{
  bool rx_done = false;

  int16_t *rx_samples_out = nullptr;

  const unsigned int rx_len = 20000;
  console_tags tags;
  struct bladerf_metadata meta;
  long sampleCount (0);
  long bytesReceived (0);
  int overruns (0);
  time_t curr_t;
  char *current_time = ctime (&curr_t);

  FILE *log = fopen (logfile_name, "a");
  FILE *outfile;

  if (strcmp (output_filename, "stdout") == 0 || strcmp (output_filename, "-") == 0)
    {
    outfile = stdout;
    }
  else
    {
    outfile = fopen (output_filename, "w+b");
    }

  fprintf (log, "RXTX ran @ %s", current_time);
  auto startTime = std::chrono::high_resolution_clock::now ();
  auto timeLimit = std::chrono::high_resolution_clock::now ();
  auto timeLastStatus = std::chrono::high_resolution_clock::now ();
  memset (&meta, 0, sizeof (meta));
  meta.flags = BLADERF_META_FLAG_RX_NOW;

  int16_t *rx_samples = new (std::nothrow) int16_t[DEFAULT_BUFFER_SIZE * 2 * sizeof (int16_t)];

  if (rx_samples == nullptr)
    {
    std::clog << "FAILED TO ALLOCATE MEM";
    }

//  rx_samples2 = (int16_t *) malloc (DEFAULT_BUFFER_SIZE * 2 * sizeof (int16_t));
//  rx_samples_out = (int16_t *) malloc (DEFAULT_BUFFER_SIZE * 2 * sizeof (int16_t));

//  if (rx_samples == nullptr)
//    {
//    perror ("malloc");
//    return BLADERF_ERR_MEM;
//    }
  if (status != 0)
    {
    perror ("Unable To Initialize BladeRF Meta Stream");
    return BLADERF_ERR_NOT_INIT;
    }
  status = bladerf_enable_module (dev, c->channel, true);
  if (status != 0)
    {
    std::cerr << "Failed to enable RX: " << bladerf_strerror (status) << std::endl;
    }
  else
    {
    while (!rx_done && status == 0)
      {
      status = bladerf_sync_rx (dev, rx_samples, DEFAULT_BUFFER_SIZE, &meta, DEFAULT_TIMEOUT_MS);

//    doBasicModulation (rx_samples,rx_samples_out,DEFAULT_BUFFER_SIZE,1,5);

//    fwrite (rx_samples_out, sizeof (*rx_samples_out), DEFAULT_BUFFER_SIZE, outfile);
//      fwrite (rx_samples, sizeof (*rx_samples) * 2, DEFAULT_BUFFER_SIZE, outfile);
      curr_stream = showStreamStats (rx_samples, meta, true, 1, startTime);
//      qDebug() << "QT DEBUG - Timestamp >>> " << meta.timestamp;

      fflush (outfile);
      }
    delete[] rx_samples;
    rx_samples = nullptr;
    }
  fclose (outfile);
  return 0;
}

/*************************************************************************************
 *
 *         QT    START META RX STREAM
 *
 *************************************************************************************/
/**
 * Peform adjustments on received samples before writing them out:
 *  (1) Mask off FPGA markers
 *  (2) Convert little-endian samples to host endianness, if needed.
 *
 *  @param  buff    Sample buffer
 *  @param  n       Number of samples
 */
int
bladeRF::device::qt_thread_start_meta_stream_rx (channel_config *c, char *output_filename, bool show_stats, int stat_level, stream_status curr_stream)
{
  int16_t *rx_samples = NULL;
  int16_t *rx_samples_out = NULL;

  const unsigned int rx_len = 20000;
  console_tags tags;

  struct bladerf_metadata meta;
  long sampleCount (0);
  long bytesReceived (0);
  int overruns (0);
  time_t curr_t;
  char *current_time = ctime (&curr_t);

  FILE *log = fopen (logfile_name, "a");
  FILE *outfile;

  if (strcmp (output_filename, "stdout") == 0 || strcmp (output_filename, "-") == 0)
    {
    outfile = stdout;
    }
  else
    {
    outfile = fopen (output_filename, "w+b");
    }

  fprintf (log, "RXTX ran @ %s", current_time);
  auto startTime = std::chrono::high_resolution_clock::now ();
  auto timeLimit = std::chrono::high_resolution_clock::now ();
  auto timeLastStatus = std::chrono::high_resolution_clock::now ();
  memset (&meta, 0, sizeof (meta));
  meta.flags = BLADERF_META_FLAG_RX_NOW;
  rx_samples = (int16_t *) malloc (DEFAULT_BUFFER_SIZE * 2 * sizeof (int16_t));
  rx_samples_out = (int16_t *) malloc (DEFAULT_BUFFER_SIZE * 2 * sizeof (int16_t));

  if (rx_samples == nullptr)
    {
    perror ("malloc");
    return BLADERF_ERR_MEM;
    }
  if (status != 0)
    {
    perror ("Unable To Initialize BladeRF Meta Stream");
    return BLADERF_ERR_NOT_INIT;
    }
  status = bladerf_enable_module (dev, c->channel, true);
  if (status != 0)
    {
    std::cerr << "Failed to enable RX: " << bladerf_strerror (status) << std::endl;
    }
  else
    {
    while (status == 0)
      {
      status = bladerf_sync_rx (dev, rx_samples, DEFAULT_BUFFER_SIZE, &meta, DEFAULT_TIMEOUT_MS);
//    doBasicModulation (rx_samples,rx_samples_out,DEFAULT_BUFFER_SIZE,1,5);
//    fwrite (rx_samples_out, sizeof (*rx_samples_out), DEFAULT_BUFFER_SIZE, outfile);
//      fwrite (rx_samples, sizeof (*rx_samples) * 2, DEFAULT_BUFFER_SIZE, outfile);
      qDebug () << "STop";
      curr_stream = showStreamStats (rx_samples, meta, true, 1, startTime);

      fflush (outfile);
      }
    }
  fclose (outfile);
  return 0;
}

/*************************************************************************************
 *
 *             START NON-META RX STREAM
 *
 *************************************************************************************/

int bladeRF::device::start_stream_rx (channel_config *c)
{
  bool rx_done;
  int16_t *rx_samples = nullptr;
  const unsigned int rx_len = 10000;
  unsigned int i;

  rx_samples = (int16_t *) malloc (rx_len * 2 * 1 * sizeof (int16_t));

  if (rx_samples == nullptr)
    {
    perror ("malloc");
    return BLADERF_ERR_MEM;
    }
  if (status != 0)
    {
    perror ("Unable To Initialize BladeRF non-meta rx stream");
    return BLADERF_ERR_NOT_INIT;
    }
  status = bladerf_enable_module (dev, c->channel, true);
  if (status != 0)
    {
    fprintf (stderr, "Failed to enable RX: %s\n", bladerf_strerror (status));
    }
  std::clog << tags.info << " RX stream in progress";
  for (i = 0; i < rx_len && status == 0; i++)
    {
    status = bladerf_sync_rx (dev, rx_samples, rx_len, nullptr, DEFAULT_TIMEOUT_MS);
    if (status != 0)
      {
      fprintf (stderr, "[ERROR] RX \"now\" failed: %s\n\n", bladerf_strerror (status));
      }
    }
  std::clog << "\n" << tags.info << " RX stream complete.";
  fflush (stdout);
  return 0;
}

/*************************************************************************************
 *
 *             INIT TX META STREAM
 *
 *************************************************************************************/

int bladeRF::device::stream_meta_init_tx (bool mimo)
{
  const unsigned int num_buffers = DEFAULT_NUM_BUFFERS;
  const unsigned int buffer_size = DEFAULT_BUFFER_SIZE; /* Must be a multiple of 1024 */
  const unsigned int num_transfers = DEFAULT_NUM_TRANSFERS;
  const unsigned int timeout_ms = DEFAULT_TIMEOUT_MS;

  status = bladerf_sync_config (dev, BLADERF_TX_X1, BLADERF_FORMAT_SC16_Q11_META, num_buffers, buffer_size, num_transfers, timeout_ms);
  status = bladerf_enable_module (dev, BLADERF_TX, true);

  if (status != 0)
    {
    fprintf (stderr, "Failed to configure TX interface: %s\n",
             bladerf_strerror (status));
    return status;
    }
  else
    {
    std::clog << tags.info << " Meta TX Stream Initialized." << std::endl;
    }
  return status;
}

/*************************************************************************************
 *  DE-INITIALIZE TRANSMISSION STREAM
 *************************************************************************************
 *
 *
 *
 *
 *************************************************************************************/

int bladeRF::device::stream_meta_deinit_rx (int16_t *samples)
{

  status = bladerf_enable_module (dev, BLADERF_TX, false);

  if (status != 0)
    {
    fprintf (stderr, "Failed to disable TX interface: %s\n",
             bladerf_strerror (status));
    return status;
    }
  else
    {
    std::clog << tags.info << "TX module Deinitialized." << std::endl;
    }
  free (samples);
  return status;
}

/*************************************************************************************
 * STOP STREAM
 *************************************************************************************
 *
 *
 *
 *
 *************************************************************************************/

void bladeRF::device::stopStream ()
{
  qDebug () << "Closing stream Now";

  status = bladerf_enable_module (dev, BLADERF_RX, false);
  if (status != 0)
    {
    std::clog << tags.error << "Failed to disable radio RX interface";
    std::clog << tags.error << stderr << "ERROR:" << bladerf_strerror (status);
//    fprintf (stderr, "Failed to disable RX interface: %s\n",
//             bladerf_strerror (status));

    }
  else
    {
    std::clog << tags.info << "RX module Deinitialized." << std::endl;
    }

  status = bladerf_enable_module (dev, BLADERF_RX, true);

}

//*************************************************************************************
//   * * * * * * * * * *     TRANSMIT - WITH META DATA     * * * * * * * * * * * * *
//*************************************************************************************


int bladeRF::device::start_meta_stream_tx (int16_t *tx_samples, channel_config *c, char *input_filename)
{
  bool tx_done;
  unsigned int tx_len = 1400;
  struct stat st;
  console_tags tags;
  unsigned int i;
  struct bladerf_metadata meta;
  time_t curr_t;
  char *current_time = ctime (&curr_t);
  FILE *log = fopen (logfile_name, "a");
  FILE *infile = fopen (input_filename, "r");
  fprintf (log, "TX ran @ %s", current_time);
  stat (input_filename, &st);
  memset (&meta, 0, sizeof (meta));
  meta.flags = BLADERF_META_FLAG_TX_BURST_START | BLADERF_META_FLAG_TX_NOW |
               BLADERF_META_FLAG_TX_BURST_END;
  tx_samples = (int16_t *) malloc (tx_len * 2 * 1 * sizeof (int16_t));

  if (tx_samples == nullptr)
    {
    perror ("malloc");
    return BLADERF_ERR_MEM;
    }
  if (status != 0)
    {
    perror ("Unable To Initialize BladeRF Meta Stream");
    return BLADERF_ERR_NOT_INIT;
    }
  status = bladerf_enable_module (dev, c->channel, true);
  if (status != 0)
    {
    std::cerr << "Failed to enable TX: " << bladerf_strerror (status) << std::endl;
    }

  for (i = 0; i < tx_len && status == 0; i++)
    {
    fread (tx_samples, sizeof (*tx_samples) * 2, DEFAULT_BUFFER_SIZE, infile);
    fflush (infile);
    status = bladerf_sync_tx (dev, tx_samples, tx_len, &meta, DEFAULT_TIMEOUT_MS);

    if (status != 0)
      {
      fprintf (stderr, "TX \"now\" failed: %s\n\n",
               bladerf_strerror (status));
      }
    else if (meta.status & BLADERF_META_STATUS_OVERRUN)
      {
      std::cerr << tags.error
                << " Overrun detected. "
                << meta.actual_count
                << " samples were actually read."
                << std::endl;
      }
    else
      {
      bladerf_timestamp curr_ts;
      status = bladerf_get_timestamp (dev, BLADERF_TX, &curr_ts);
      std::clog << tags.info
                << " TX Samples=" << i
                << " ts=" << curr_ts
                << " sample=" << *tx_samples
                << std::endl;
      usleep (5000);
      }
    }

  if (status == 0)
    {
    status = bladerf_get_timestamp (dev, BLADERF_TX, &meta.timestamp);
    if (status != 0)
      {
      std::clog << tags.warning << " Failed to get current timestamp: " << bladerf_strerror (status);
      return status;
      }
    else
      {
      status = wait_for_timestamp (dev, BLADERF_TX, meta.timestamp + 2 * tx_len, DEFAULT_TIMEOUT_MS);
      if (status != 0)
        {
        std::clog << tags.warning << " Failed to wait for timestamp: " << bladerf_strerror (status);
        return status;
        }
      }
    }

  return 0;
}
