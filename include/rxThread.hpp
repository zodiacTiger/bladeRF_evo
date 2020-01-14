//
// Created by ZodiacTiger on 2019-06-27.
//

#ifndef BLADERF_EVO_RXTHREAD_HPP
#define BLADERF_EVO_RXTHREAD_HPP

#include <QThread>
//#include <device.hpp>
#include <QDebug>
#include <iostream>
#include <QMainWindow>
#include "mainwindow.h"

 class rxThread : public QThread{
//  Q_OBJECT

 public:
  rxThread();
  volatile bool stopped;
  ~rxThread ();
  void stop();

 private:
  void run();
 public slots:
//  void streamInfoUpdate(QString);
};



#endif //BLADERF_EVO_RXTHREAD_HPP
