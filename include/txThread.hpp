//
// Created by ZodiacTiger on 2019-08-05.
//

#ifndef BLADERF_EVO_TXTHREAD_HPP
#define BLADERF_EVO_TXTHREAD_HPP

#include <QThread>
//#include <device.hpp>
#include <QDebug>
#include <iostream>
#include <QMainWindow>
#include "mainwindow.h"

class TxThread : public QThread{

 public:
  TxThread();
  volatile bool stopped;
  ~TxThread ();
  void stop();

 private:
  void run();
 public slots:
};

#endif //BLADERF_EVO_TXTHREAD_HPP
