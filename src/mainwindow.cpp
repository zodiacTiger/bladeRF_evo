

#ifndef MAINWINDOW_H__
#define MAINWINDOW_H__

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "session.hpp"
#include <logging.hpp>
#include <iostream>
#include <libbladeRF.h>
#include <QLabel>
#include <QDate>
#include <QVBoxLayout>
#include <QSplashScreen>
#include <rxThread.hpp>
#include <device.hpp>
#include <unistd.h>

int gain = 45;

QString default_divider = "================================================================";
QString rx_divider = "====================[     RX STREAM STARTED    ]======================";
QString tx_divider = "=========================[     TX    ]========================";
QString dev_status_divider = "=======================[    DEVICE STATUS    ]========================";
QString dev_info_divider = "========================[    DEVICE INFO    ]=========================";

auto rx = new rxThread ();

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent),
                                           ui (new Ui::MainWindow)
{

  int status = 0;
  ui->setupUi (this);
  setWindowTitle ("BladeRF RXTX");
  ui->stopButton->setEnabled (false);

  QSplashScreen *splash = new QSplashScreen;
  splash->setPixmap (QPixmap ("/Users/zero5/CLionProjects/bladeRF_evo/images/bladeRF_bg.png"));
  splash->show ();
  QFont splashFont;
  splashFont.setFamily (QString::fromUtf8 ("UbuntuMono NF"));
  splashFont.setPointSize (13);

  Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
  splash->setFont (splashFont);
  splash->showMessage (QObject::tr ("Finding Device"),
                       topRight, Qt::white);
  if (status == 0)
    {
    splash->showMessage (QObject::tr ("BladeRF Device Found"),
                         topRight, Qt::white);
    }
  if (status == 0)
    {
    splash->showMessage (QObject::tr ("Initialize default settings"),
                         topRight, Qt::white);
    }
  splash->finish (this);
  delete splash;
  bladerf_close (bladeRF::device::getDevice ());
  ui->gainValue_RX1->setText (QString::number (config_rx0.gain));
  ui->gainValue_RX2->setText (QString::number (config_rx1.gain));

  ui->gainValue_TX1->setText (QString::number (config_tx0.gain));
  ui->gainValue_TX2->setText (QString::number (config_tx1.gain));

  ui->rxFreqTextBox->setText (QString::number (config_rx0.frequency));
  ui->rxFreqTextBox_2->setText (QString::number (config_rx1.frequency));

  ui->txFreqTextBox->setText (QString::number (config_tx0.frequency));
  ui->txFreqTextBox_2->setText (QString::number (config_tx1.frequency));

  show_connected_device ();

}

MainWindow::~MainWindow ()
{
  delete ui;
}

int MainWindow::show_connected_device ()
{
  int status;

  QString info = ui->deviceInfo->toPlainText ();

  const char *str_chan;
  const struct bladerf_range *gain_range;
  const struct bladerf_range *freq_range;
  const struct bladerf_range *samplerate_range;
  const struct bladerf_range *bandwidth_range;

  struct bladerf_version fpga_ver;
  struct bladerf_version lib_ver;
  uint16_t trim_dac_val;

  bladerf_devinfo dev_info;
  bladerf_init_devinfo (&dev_info);
  status = bladerf_open_with_devinfo (&dev, &dev_info);
  status = bladerf_get_devinfo (dev, &dev_info);
  status = bladerf_fpga_version (dev, &fpga_ver);
  bladerf_dev_speed speed;
  speed = bladerf_device_speed (dev);
  bool fpga_configured = false;
  bladerf_version (&lib_ver);

  if (bladerf_is_fpga_configured (dev))
    {
    fpga_configured = true;
    }

  status = bladerf_trim_dac_read (dev, &trim_dac_val);
  bladerf_frequency rx_0_freq;
  int ret;
  size_t const rx_ch_count = bladerf_get_channel_count (dev, BLADERF_RX);
  size_t const tx_ch_count = bladerf_get_channel_count (dev, BLADERF_TX);
  ret = bladerf_get_gain_range (dev, BLADERF_CHANNEL_RX(0), &gain_range);
  ret = bladerf_get_frequency_range (dev, BLADERF_CHANNEL_RX (0), &freq_range);
  ret = bladerf_get_sample_rate_range (dev, BLADERF_CHANNEL_RX (0), &samplerate_range);
  ret = bladerf_get_bandwidth_range (dev, BLADERF_CHANNEL_RX (0), &bandwidth_range);
  ret = bladerf_get_sample_rate_range (dev, BLADERF_CHANNEL_RX (0), &samplerate_range);
  ret = bladerf_get_bandwidth_range (dev, BLADERF_CHANNEL_RX (0), &bandwidth_range);
  ret = bladerf_get_gain (dev, BLADERF_CHANNEL_RX (0), &gain);
  ret = bladerf_get_frequency (dev, BLADERF_CHANNEL_RX (0), &rx_0_freq);

  if (status == 0)
    {
    info.append (
        "Connected Device: "
        + QString::fromLatin1 (dev_info.manufacturer)
        + " "
        + QString::fromLatin1 (dev_info.product)
        + " \n"
        + "Serial #: " + QString::fromLatin1 (dev_info.serial)
        + "\n"
        + "FPGA v." + QString::number (fpga_ver.major) + "." + QString::number (fpga_ver.minor) + "."
        + QString::number (fpga_ver.patch)
        + " BladeRF Library v." + QString::fromLatin1 (lib_ver.describe)
        + " \n"
        + "Backend : " + QString::fromLatin1 (bladerf_backend_str (dev_info.backend))
        + "\n"
        + "Instance(" + QString::number (dev_info.instance) + ") using "
        + "USB Addr: [" + QString::number (dev_info.usb_addr) + ":" + QString::number (dev_info.usb_bus) + "]"
        + "Device Speed: " + QString::number (speed)
    );
    ui->deviceInfo->appendPlainText (dev_info_divider);
    ui->deviceInfo->appendPlainText (info);
    ui->deviceInfo->appendPlainText ("");
    ui->deviceInfo->appendPlainText (dev_status_divider);
    if (fpga_configured)
      ui->deviceInfo->appendPlainText (">> FPGA successfully configured");
    int iq_bal_coeff = 1000;
    status = bladeRF::device::set_iq_balance (iq_bal_coeff, BLADERF_CHANNEL_RX (0));
    if (status == 0)
      {
      ui->deviceInfo->appendPlainText (">> IQ Balance enabled, coeff=" + QString::number (iq_bal_coeff));
      }
    ui->deviceInfo->appendPlainText (">> Trim DAC= " + QString::number (trim_dac_val));
    ui->deviceInfo->appendPlainText (">> Gain RX 0 = " + QString::number (gain));
    ui->deviceInfo->appendPlainText (">> Freq RX 0 = " + QString::number (rx_0_freq));

    ui->deviceInfo->appendPlainText (default_divider);
    bladerf_close (dev);

    }
  else
    {
    ui->deviceInfo->setPlainText ("!!ERROR: No Device Found!!");
    }
  return status;
}

void MainWindow::on_deviceButton_clicked ()
{

  qDebug () << "Yay, device info!";
  ui->deviceInfo->clear ();
  ui->rxButton->setEnabled (false);
  ui->txButton->setEnabled (false);
  ui->stopButton->setEnabled (false);
  show_connected_device ();
  ui->rxButton->setEnabled (true);
  ui->txButton->setEnabled (true);
}

void MainWindow::on_txButton_clicked ()
{
  bladerf_devinfo dev_info;
  qDebug () << "yay!! Go tx";
  bladeRF::device::init (&dev_info);
  qDebug () << QString::fromLatin1 (dev_info.serial);

  ui->deviceInfo->appendPlainText (">> TX stream started");
  ui->txButton->setEnabled (false);
  ui->deviceButton->setEnabled (false);
  ui->stopButton->setEnabled (true);

  bladerf_close (bladeRF::device::getDevice ());
  dev = nullptr;
  ui->rxButton->setEnabled (true);
  ui->deviceButton->setEnabled (true);
  ui->stopButton->setEnabled (true);
}

void MainWindow::on_rxButton_clicked ()
{

  int status = 0;
//  QList<QThread*> rxThreads;
  qDebug () << "yay!! Go rx";

//  Session *ses = new Session(1);

  ui->rxButton->setEnabled (false);
  ui->deviceButton->setEnabled (false);
  ui->stopButton->setEnabled (true);

  rx->start (rxThread::HighestPriority);
  qDebug () << "rx thread created";

  rx->moveToThread (rx);
  rx->isRunning () ? qDebug () << "rx thread running" : qDebug () << "rx thread not running";

  ui->deviceInfo->clear ();
  ui->deviceInfo->appendPlainText (rx_divider);
  ui->deviceInfo->appendPlainText (">> RX stream ");
  ui->deviceInfo->appendPlainText (">> @ ");

  connect (rx, SIGNAL(streamInfoUpdate (QString)), this, SLOT(on_deviceInfoUpdate (QString)));
}

void MainWindow::on_deviceInfo_update (QString)
{
  ui->deviceInfo->appendPlainText ("afdadsf");
}

void MainWindow::on_quitButton_clicked ()
{
  this->close ();
  bladeRF::device::close ();
  exit (0);
}

void MainWindow::on_stopButton_clicked ()
{
  int status = 0;
  ui->stopButton->setEnabled (false);
  ui->rxButton->setEnabled (false);
  ui->txButton->setEnabled (false);
  ui->deviceButton->setEnabled (false);

  if (!rx->isRunning ())
    {
    ui->deviceInfo->appendPlainText (">> No stream/thread is running.");
    qDebug () << "No running threads.  Ignoring stop command.";
    }
  else
    {
    qDebug () << "Stop Button. Trying to stop thread/stream.";

    }
  if (rx->isRunning ())
    {
    rx->stop ();
    rx->terminate ();

    while (!rx->isFinished () && !rx->stopped)
      ui->rxButton->setEnabled (false);
    if (!rx->isRunning ())
      {
      ui->deviceInfo->appendPlainText (">> Stream Stopped");

      ui->rxButton->setEnabled (true);
      qDebug () << "Thread Stopped.";
      }
    }
  ui->rxButton->setEnabled (true);
  ui->txButton->setEnabled (true);
  ui->deviceButton->setEnabled (true);
}

void MainWindow::on_gainChanged_RX1 ()
{

  bladerf_gain gain;
  bladerf_gain curr_gain;
  bladerf_channel ch = BLADERF_CHANNEL_RX (0);
  size_t num_of_devs;
  struct bladerf_devinfo *dev_list;
  num_of_devs = bladerf_get_device_list (&dev_list);

  int ret = 0;

  int val = ui->gainSlider_RX1->value ();
  config_rx0.gain = val;
  gain = val;

//  ret = bladerf_set_gain (dev,ch,30);
//  ret = bladerf_get_gain (dev,ch,&curr_gain);

  ui->gainValue_RX1->setText (QString::number (val));
  ui->deviceInfo->appendPlainText ("Gain >> " + QString::number (val));

  qDebug () << "GAIN >>> " << gain;
}

void MainWindow::on_gainChanged_RX2 ()
{
  bladerf_gain gain;

  int val = ui->gainSlider_RX2->value ();
  ui->gainValue_RX2->setText (QString::number (val));
  qDebug () << "RX2 Gain Changed to " << QString::number (val);

}

void MainWindow::on_gainChanged_TX1 ()
{
  bladerf_gain gain = 10;
  int val = ui->gainSlider_TX1->value ();
  ui->gainValue_TX1->setText (QString::number (val));
  qDebug () << "TX1 Gain Changed to " << QString::number (val);
}

void MainWindow::on_gainChanged_TX2 ()
{
  bladerf_gain gain = 10;
  int val = ui->gainSlider_TX2->value ();
  ui->gainValue_TX2->setText (QString::number (val));
  qDebug () << "TX2 Gain Changed to " << QString::number (val);
}

#endif