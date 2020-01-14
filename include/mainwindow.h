//
// Created by ZodiacTiger on 2019-06-10.
//

#ifndef BLADERF_EVO_MAINWINDOW_H
#define BLADERF_EVO_MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <libbladeRF.h>
#include <device.hpp>

namespace Ui {
class MainWindow;
}

 class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

 public slots:

 private:
  Ui::MainWindow *ui;

 private slots:
  void on_quitButton_clicked();
  void on_deviceButton_clicked();
  void on_rxButton_clicked();
  void on_txButton_clicked();
//  void enableRXTXChanged();

  void on_stopButton_clicked();
  void on_gainChanged_RX1();
  void on_gainChanged_RX2();
  void on_gainChanged_TX1();
  void on_gainChanged_TX2();
  void on_deviceInfo_update(QString);

  int show_connected_device();

  signals:



 };

#endif //BLADERF_EVO_MAINWINDOW_H
