#include <TxThread.hpp>
#include <device.hpp>
#include <QDebug>
#include <iostream>
#include <libbladeRF.h>

static int status = 0;



TxThread::TxThread ()
{

}

TxThread::~TxThread ()
{
}


void TxThread::run ()
{

  qDebug () << "yay!! Go rx thread";

  if(this->isRunning ())
    {
    qDebug()<<"Thread is running.";
    }
  config_rx0.channel = BLADERF_CHANNEL_RX (0);
  config_rx0.samplerate = DEFAULT_SAMPLERATE;
  config_rx0.bandwidth = DEFAULT_BANDWIDTH;
  config_rx0.frequency = DEFAULT_WIFI_RX;
  config_rx0.gain = 52;
  int16_t *rx_samples = new(std::nothrow) int16_t[DEFAULT_BUFFER_SIZE * 2 * sizeof (int16_t)];

  stream_status *curr;
  const unsigned int num_buffers = DEFAULT_NUM_BUFFERS;
  const unsigned int buffer_size = DEFAULT_BUFFER_SIZE; /* Must be a multiple of 1024 */
  const unsigned int num_transfers = DEFAULT_NUM_TRANSFERS;
  const unsigned int timeout_ms = DEFAULT_TIMEOUT_MS;

  bladerf_init_devinfo (&dev_info);
  status = bladerf_open_with_devinfo (&dev, &dev_info);
  status = bladerf_set_frequency (dev, config_rx0.channel, config_rx0.frequency);
  status = bladerf_set_gain(dev, config_rx0.gain, config_rx0.gain);

  status = bladerf_sync_config (dev, BLADERF_RX_X1, BLADERF_FORMAT_SC16_Q11_META, num_buffers, buffer_size, num_transfers, timeout_ms);
  status = bladerf_enable_module (dev, BLADERF_CHANNEL_RX (0), true);

  int16_t *rx_samples_out = nullptr;

  const unsigned int rx_len = 20000;
  console_tags tags;
  struct bladerf_metadata meta;
  memset (&meta, 0, sizeof (meta));

  long sampleCount (0);
  long bytesReceived (0);
  int overruns (0);
  time_t curr_t;
  char *current_time = ctime (&curr_t);
  char *output_filename = "/tmp/tosdfasfa.sfd";
  FILE *log = fopen (logfile_name, "a");
  FILE *outfile;

  if(strcmp (output_filename,"stdout") == 0 || strcmp(output_filename,"-") == 0)
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
  meta.flags = BLADERF_META_FLAG_RX_NOW;


  if(rx_samples == nullptr)
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
  stream_status curr_stream;
  if (status != 0)
    {
    std::cerr << "Failed to enable RX: " << bladerf_strerror (status) << std::endl;
    }
  else{
    while (status == 0)
      {
      status = bladerf_sync_rx (dev, rx_samples, DEFAULT_BUFFER_SIZE, &meta, DEFAULT_TIMEOUT_MS);

//    doBasicModulation (rx_samples,rx_samples_out,DEFAULT_BUFFER_SIZE,1,5);

//    fwrite (rx_samples_out, sizeof (*rx_samples_out), DEFAULT_BUFFER_SIZE, outfile);
//      fwrite (rx_samples, sizeof (*rx_samples) * 2, DEFAULT_BUFFER_SIZE, outfile);
      curr_stream = showStreamStats (rx_samples,meta,true,1,startTime);
//      qDebug() << "QT DEBUG - Timestamp >>> " << meta.timestamp << " Sample >>> " << *rx_samples;
//      fflush (outfile);
      }
    }
  fclose (outfile);
}

void TxThread::stop ()
{
  this->setPriority (TxThread::HighestPriority);
  bladerf_close (dev);
}