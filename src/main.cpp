/*
 *
 *
 *
 *
 *
 * */

#include <iostream>
#include <QApplication>

#include "device.hpp"
#include "commandLineArgs.hpp"

#include "mainwindow.h"
#include "ui_mainwindow.h"

static sig_atomic_t devDone = false;


void signalHandler( int signum )
{

  std::cout << "\nInterrupt signal (" << signum << ") received.\n";
  std::cout << "\nClosing Program";


  exit(signum);
}

int main (int argc, char *argv[])
{
  signal(SIGINT, signalHandler);

  if(argc > 1)                        // if no command line arguments are given, gui is started.
    {
      parseCommandLine (argc,argv);
    }
  else
    {
      QApplication app(argc, argv);

    MainWindow w;
      w.show();

      return app.exec();
    }
}
