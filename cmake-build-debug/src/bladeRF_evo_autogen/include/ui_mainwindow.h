/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *rxButton;
    QPushButton *txButton;
    QPushButton *stopButton;
    QFrame *frame;
    QSlider *gainSlider_RX2;
    QLabel *label_4;
    QLabel *gainValue_RX2;
    QLabel *label_5;
    QLabel *gainValue_TX1;
    QSlider *gainSlider_TX1;
    QLabel *label_6;
    QLabel *gainValue_TX2;
    QSlider *gainSlider_TX2;
    QLabel *label_7;
    QLabel *gainValue_RX1;
    QSlider *gainSlider_RX1;
    QCheckBox *checkBox;
    QFrame *frame_2;
    QLineEdit *txFreqTextBox;
    QLineEdit *rxFreqTextBox;
    QLabel *rxFreqInput;
    QLabel *label;
    QFrame *frame_3;
    QLineEdit *txFreqTextBox_2;
    QLineEdit *rxFreqTextBox_2;
    QLabel *rxFreqInput_2;
    QLabel *label_2;
    QPushButton *quitButton;
    QPushButton *deviceButton;
    QPlainTextEdit *deviceInfo;
    QComboBox *comboBox;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(674, 628);
        MainWindow->setMinimumSize(QSize(674, 628));
        MainWindow->setMaximumSize(QSize(674, 628));
        QFont font;
        font.setFamily(QString::fromUtf8("Titillium Web"));
        font.setPointSize(13);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        MainWindow->setFont(font);
        MainWindow->setStyleSheet(QString::fromUtf8("#MainWindow{\n"
"background: url(\"../images/bg.png\");\n"
"background-color: rgb(69,69,69);\n"
"border: 4px solid rgb(21, 166, 167);\n"
"	font: 13pt \"Titillium Web\";\n"
"}\n"
""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(61, 61, 61)\n"
"\n"
"\n"
""));
        rxButton = new QPushButton(centralWidget);
        rxButton->setObjectName(QString::fromUtf8("rxButton"));
        rxButton->setGeometry(QRect(480, 100, 91, 61));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Titillium Web"));
        font1.setPointSize(14);
        rxButton->setFont(font1);
        rxButton->setFocusPolicy(Qt::TabFocus);
        rxButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 2px solid rgb(172, 169, 174);\n"
"            border-radius: 6px;\n"
"            background-color:rgb(108, 108, 108);\n"
"            color: rgb(238, 238, 238);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color: rgb(81, 81, 81);\n"
"	color: rgb(23, 188, 255)\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"border-color: rgb(129, 128, 128);\n"
"background-color: rgb(80, 82, 82);\n"
"color: rgb(128, 130, 130);\n"
"}"));
        txButton = new QPushButton(centralWidget);
        txButton->setObjectName(QString::fromUtf8("txButton"));
        txButton->setGeometry(QRect(580, 100, 81, 61));
        txButton->setFont(font1);
        txButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 2px solid rgb(172, 169, 174);\n"
"            border-radius: 6px;\n"
"            background-color:rgb(108, 108, 108);\n"
"            color: rgb(238, 238, 238);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color: rgb(81, 81, 81);\n"
"	color: rgb(23, 188, 255)\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"border-color: rgb(129, 128, 128);\n"
"background-color: rgb(80, 82, 82);\n"
"color: rgb(128, 130, 130);\n"
"}"));
        stopButton = new QPushButton(centralWidget);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setEnabled(false);
        stopButton->setGeometry(QRect(480, 170, 181, 51));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Titillium Web"));
        font2.setPointSize(13);
        stopButton->setFont(font2);
        stopButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 2px solid rgb(172, 169, 174);\n"
"            border-radius: 6px;\n"
"            background-color:rgb(108, 108, 108);\n"
"            color: rgb(238, 238, 238);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color: rgb(81, 81, 81);\n"
"	color: rgb(23, 188, 255);\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"border-color: rgb(129, 128, 128);\n"
"background-color: rgb(80, 82, 82);\n"
"color: rgb(128, 130, 130);\n"
"}"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 400, 461, 191));
        frame->setAutoFillBackground(false);
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(132, 132, 132);\n"
"border-radius: 8px;"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gainSlider_RX2 = new QSlider(frame);
        gainSlider_RX2->setObjectName(QString::fromUtf8("gainSlider_RX2"));
        gainSlider_RX2->setGeometry(QRect(90, 69, 321, 21));
        gainSlider_RX2->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background-color: #333;\n"
"border-radius: 4px;\n"
"padding-top: 2px;\n"
"\n"
""));
        gainSlider_RX2->setMaximum(70);
        gainSlider_RX2->setSliderPosition(40);
        gainSlider_RX2->setOrientation(Qt::Horizontal);
        gainSlider_RX2->setInvertedAppearance(false);
        gainSlider_RX2->setInvertedControls(false);
        gainSlider_RX2->setTickPosition(QSlider::NoTicks);
        gainSlider_RX2->setTickInterval(1);
        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 70, 71, 21));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Titillium Web"));
        font3.setPointSize(12);
        label_4->setFont(font3);
        label_4->setFocusPolicy(Qt::NoFocus);
        label_4->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background-color: #333;\n"
"\n"
""));
        label_4->setAlignment(Qt::AlignCenter);
        gainValue_RX2 = new QLabel(frame);
        gainValue_RX2->setObjectName(QString::fromUtf8("gainValue_RX2"));
        gainValue_RX2->setGeometry(QRect(420, 70, 31, 21));
        gainValue_RX2->setFont(font1);
        gainValue_RX2->setStyleSheet(QString::fromUtf8("            border-radius: 3px;\n"
"            background-color:rgb(108, 108, 108);\n"
"            color: rgb(238, 238, 238);\n"
"		padding: 4px;\n"
""));
        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 120, 71, 21));
        label_5->setFont(font3);
        label_5->setFocusPolicy(Qt::NoFocus);
        label_5->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background-color: #333;\n"
"\n"
""));
        label_5->setAlignment(Qt::AlignCenter);
        gainValue_TX1 = new QLabel(frame);
        gainValue_TX1->setObjectName(QString::fromUtf8("gainValue_TX1"));
        gainValue_TX1->setGeometry(QRect(420, 120, 31, 21));
        gainValue_TX1->setFont(font1);
        gainValue_TX1->setStyleSheet(QString::fromUtf8("            border-radius: 3px;\n"
"            background-color:rgb(108, 108, 108);\n"
"            color: rgb(238, 238, 238);\n"
"		padding: 4px;\n"
""));
        gainSlider_TX1 = new QSlider(frame);
        gainSlider_TX1->setObjectName(QString::fromUtf8("gainSlider_TX1"));
        gainSlider_TX1->setGeometry(QRect(90, 119, 321, 21));
        gainSlider_TX1->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background-color: #333;\n"
"border-radius: 4px;\n"
"padding-top: 2px;\n"
"\n"
""));
        gainSlider_TX1->setMaximum(70);
        gainSlider_TX1->setSliderPosition(40);
        gainSlider_TX1->setOrientation(Qt::Horizontal);
        gainSlider_TX1->setInvertedAppearance(false);
        gainSlider_TX1->setInvertedControls(false);
        gainSlider_TX1->setTickPosition(QSlider::NoTicks);
        gainSlider_TX1->setTickInterval(1);
        label_6 = new QLabel(frame);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 150, 71, 21));
        label_6->setFont(font3);
        label_6->setFocusPolicy(Qt::NoFocus);
        label_6->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background-color: #333;\n"
"\n"
""));
        label_6->setAlignment(Qt::AlignCenter);
        gainValue_TX2 = new QLabel(frame);
        gainValue_TX2->setObjectName(QString::fromUtf8("gainValue_TX2"));
        gainValue_TX2->setGeometry(QRect(420, 150, 31, 21));
        gainValue_TX2->setFont(font1);
        gainValue_TX2->setStyleSheet(QString::fromUtf8("            border-radius: 3px;\n"
"            background-color:rgb(108, 108, 108);\n"
"            color: rgb(238, 238, 238);\n"
"		padding: 4px;\n"
""));
        gainSlider_TX2 = new QSlider(frame);
        gainSlider_TX2->setObjectName(QString::fromUtf8("gainSlider_TX2"));
        gainSlider_TX2->setGeometry(QRect(90, 149, 321, 21));
        gainSlider_TX2->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background-color: #333;\n"
"border-radius: 4px;\n"
"padding-top: 2px;\n"
"\n"
""));
        gainSlider_TX2->setMaximum(70);
        gainSlider_TX2->setSliderPosition(40);
        gainSlider_TX2->setOrientation(Qt::Horizontal);
        gainSlider_TX2->setInvertedAppearance(false);
        gainSlider_TX2->setInvertedControls(false);
        gainSlider_TX2->setTickPosition(QSlider::NoTicks);
        gainSlider_TX2->setTickInterval(1);
        label_7 = new QLabel(frame);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(30, 41, 71, 21));
        label_7->setFont(font3);
        label_7->setFocusPolicy(Qt::NoFocus);
        label_7->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background-color: #333;\n"
"\n"
""));
        label_7->setAlignment(Qt::AlignCenter);
        gainValue_RX1 = new QLabel(frame);
        gainValue_RX1->setObjectName(QString::fromUtf8("gainValue_RX1"));
        gainValue_RX1->setGeometry(QRect(420, 41, 31, 21));
        gainValue_RX1->setFont(font1);
        gainValue_RX1->setStyleSheet(QString::fromUtf8("            border-radius: 3px;\n"
"            background-color:rgb(108, 108, 108);\n"
"            color: rgb(238, 238, 238);\n"
"		padding: 4px;\n"
""));
        gainSlider_RX1 = new QSlider(frame);
        gainSlider_RX1->setObjectName(QString::fromUtf8("gainSlider_RX1"));
        gainSlider_RX1->setGeometry(QRect(90, 40, 321, 21));
        gainSlider_RX1->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background-color: #333;\n"
"border-radius: 4px;\n"
"padding-top: 2px;\n"
"\n"
""));
        gainSlider_RX1->setMaximum(70);
        gainSlider_RX1->setSliderPosition(40);
        gainSlider_RX1->setOrientation(Qt::Horizontal);
        gainSlider_RX1->setInvertedAppearance(false);
        gainSlider_RX1->setInvertedControls(false);
        gainSlider_RX1->setTickPosition(QSlider::NoTicks);
        gainSlider_RX1->setTickInterval(1);
        checkBox = new QCheckBox(frame);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(30, 10, 71, 21));
        checkBox->setFont(font1);
#if QT_CONFIG(tooltip)
        checkBox->setToolTip(QString::fromUtf8(""));
#endif // QT_CONFIG(tooltip)
        checkBox->setAutoFillBackground(false);
        checkBox->setStyleSheet(QString::fromUtf8("QCheckBox\n"
"{\n"
"\n"
"            border-radius: 3px;\n"
"            background-color:rgb(108, 108, 108);\n"
"            color: rgb(238, 238, 238);\n"
"		padding: 4px;\n"
"\n"
"		\n"
"}\n"
"QCheckBox:focus\n"
"{\n"
"  border-color: rgb(73, 255, 31);\n"
"}\n"
"\n"
"\n"
"\n"
"QPushButton:hover{\n"
"	background-color: rgb(81, 81, 81);\n"
"	color: rgb(23, 188, 255)\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"border-color: rgb(129, 128, 128);\n"
"background-color: rgb(80, 82, 82);\n"
"color: rgb(128, 130, 130);\n"
"}"));
        checkBox->setChecked(false);
        label_4->raise();
        gainSlider_RX2->raise();
        gainValue_RX2->raise();
        label_5->raise();
        gainValue_TX1->raise();
        gainSlider_TX1->raise();
        label_6->raise();
        gainValue_TX2->raise();
        gainSlider_TX2->raise();
        label_7->raise();
        gainValue_RX1->raise();
        gainSlider_RX1->raise();
        checkBox->raise();
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(480, 400, 181, 91));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(132, 132, 132);\n"
"border-radius: 8px;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        txFreqTextBox = new QLineEdit(frame_2);
        txFreqTextBox->setObjectName(QString::fromUtf8("txFreqTextBox"));
        txFreqTextBox->setGeometry(QRect(60, 50, 91, 20));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(txFreqTextBox->sizePolicy().hasHeightForWidth());
        txFreqTextBox->setSizePolicy(sizePolicy);
        txFreqTextBox->setFont(font1);
        txFreqTextBox->setFocusPolicy(Qt::ClickFocus);
        txFreqTextBox->setStyleSheet(QString::fromUtf8("background-color: rgb(50, 50, 50);\n"
"border: 1px solid rgb(177, 180, 180);\n"
"border-radius: 4px;\n"
"color: rgb(232, 236, 236);\n"
""));
        txFreqTextBox->setMaxLength(10);
        txFreqTextBox->setFrame(true);
        txFreqTextBox->setAlignment(Qt::AlignCenter);
        txFreqTextBox->setClearButtonEnabled(false);
        rxFreqTextBox = new QLineEdit(frame_2);
        rxFreqTextBox->setObjectName(QString::fromUtf8("rxFreqTextBox"));
        rxFreqTextBox->setGeometry(QRect(60, 20, 91, 21));
        sizePolicy.setHeightForWidth(rxFreqTextBox->sizePolicy().hasHeightForWidth());
        rxFreqTextBox->setSizePolicy(sizePolicy);
        rxFreqTextBox->setFont(font1);
        rxFreqTextBox->setFocusPolicy(Qt::ClickFocus);
        rxFreqTextBox->setAcceptDrops(true);
        rxFreqTextBox->setStyleSheet(QString::fromUtf8("background-color: rgb(50, 50, 50);\n"
"border: 1px solid rgb(177, 180, 180);\n"
"border-radius: 4px;\n"
"color: rgb(232, 236, 236);\n"
""));
        rxFreqTextBox->setMaxLength(10);
        rxFreqTextBox->setFrame(true);
        rxFreqTextBox->setAlignment(Qt::AlignCenter);
        rxFreqTextBox->setClearButtonEnabled(false);
        rxFreqInput = new QLabel(frame_2);
        rxFreqInput->setObjectName(QString::fromUtf8("rxFreqInput"));
        rxFreqInput->setGeometry(QRect(10, 10, 71, 31));
        rxFreqInput->setFont(font3);
        rxFreqInput->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background: none;\n"
""));
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 50, 56, 16));
        label->setFont(font3);
        label->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background: none;\n"
""));
        frame_3 = new QFrame(centralWidget);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(480, 500, 181, 91));
        frame_3->setStyleSheet(QString::fromUtf8("background-color: rgb(132, 132, 132);\n"
"border-radius: 8px;"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        txFreqTextBox_2 = new QLineEdit(frame_3);
        txFreqTextBox_2->setObjectName(QString::fromUtf8("txFreqTextBox_2"));
        txFreqTextBox_2->setGeometry(QRect(60, 50, 91, 21));
        sizePolicy.setHeightForWidth(txFreqTextBox_2->sizePolicy().hasHeightForWidth());
        txFreqTextBox_2->setSizePolicy(sizePolicy);
        txFreqTextBox_2->setFont(font1);
        txFreqTextBox_2->setFocusPolicy(Qt::ClickFocus);
        txFreqTextBox_2->setStyleSheet(QString::fromUtf8("background-color: rgb(50, 50, 50);\n"
"border: 1px solid rgb(177, 180, 180);\n"
"border-radius: 4px;\n"
"color: rgb(232, 236, 236);\n"
""));
        txFreqTextBox_2->setMaxLength(10);
        txFreqTextBox_2->setFrame(true);
        txFreqTextBox_2->setAlignment(Qt::AlignCenter);
        txFreqTextBox_2->setClearButtonEnabled(false);
        rxFreqTextBox_2 = new QLineEdit(frame_3);
        rxFreqTextBox_2->setObjectName(QString::fromUtf8("rxFreqTextBox_2"));
        rxFreqTextBox_2->setGeometry(QRect(60, 20, 91, 21));
        sizePolicy.setHeightForWidth(rxFreqTextBox_2->sizePolicy().hasHeightForWidth());
        rxFreqTextBox_2->setSizePolicy(sizePolicy);
        rxFreqTextBox_2->setFont(font1);
        rxFreqTextBox_2->setFocusPolicy(Qt::ClickFocus);
        rxFreqTextBox_2->setAcceptDrops(true);
        rxFreqTextBox_2->setStyleSheet(QString::fromUtf8("background-color: rgb(50, 50, 50);\n"
"border: 1px solid rgb(177, 180, 180);\n"
"border-radius: 4px;\n"
"color: rgb(232, 236, 236);\n"
""));
        rxFreqTextBox_2->setMaxLength(10);
        rxFreqTextBox_2->setFrame(true);
        rxFreqTextBox_2->setAlignment(Qt::AlignCenter);
        rxFreqTextBox_2->setClearButtonEnabled(false);
        rxFreqInput_2 = new QLabel(frame_3);
        rxFreqInput_2->setObjectName(QString::fromUtf8("rxFreqInput_2"));
        rxFreqInput_2->setGeometry(QRect(10, 10, 71, 31));
        rxFreqInput_2->setFont(font3);
        rxFreqInput_2->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background: none;\n"
""));
        label_2 = new QLabel(frame_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 50, 56, 16));
        label_2->setFont(font3);
        label_2->setStyleSheet(QString::fromUtf8("color: #f3f3f3;\n"
"background: none;\n"
""));
        quitButton = new QPushButton(centralWidget);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setGeometry(QRect(640, 10, 21, 21));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Titillium Web"));
        font4.setPointSize(14);
        font4.setBold(true);
        font4.setWeight(75);
        quitButton->setFont(font4);
        quitButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 2px solid rgb(172, 169, 174);\n"
"            border-radius: 4px;\n"
"            background-color:rgb(105, 107, 107);\n"
"            color: rgb(255, 255, 255);\n"
"		font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color: rgb(81, 81, 81);\n"
"	border-color : rgb(252, 51, 22);\n"
"	color: rgb(252, 61, 28)\n"
"}"));
        deviceButton = new QPushButton(centralWidget);
        deviceButton->setObjectName(QString::fromUtf8("deviceButton"));
        deviceButton->setGeometry(QRect(480, 40, 181, 51));
        deviceButton->setFont(font3);
        deviceButton->setCursor(QCursor(Qt::ArrowCursor));
        deviceButton->setAutoFillBackground(false);
        deviceButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"border: 2px solid rgb(172, 169, 174);\n"
"            border-radius: 6px;\n"
"            background-color:rgb(105, 107, 107);\n"
"\n"
"            color: rgb(238, 238, 238);\n"
"\n"
"text-transform: uppercase;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color: rgb(81, 81, 81);\n"
"	color: rgb(23, 188, 255)\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"border-color: rgb(129, 128, 128);\n"
"background-color: rgb(80, 82, 82);\n"
"color: rgb(128, 130, 130);\n"
"}"));
        deviceInfo = new QPlainTextEdit(centralWidget);
        deviceInfo->setObjectName(QString::fromUtf8("deviceInfo"));
        deviceInfo->setGeometry(QRect(10, 40, 461, 351));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Titillium Web"));
        font5.setPointSize(11);
        deviceInfo->setFont(font5);
        deviceInfo->setStyleSheet(QString::fromUtf8("background-color: rgb(37, 37, 37);\n"
"border: 1px solid rgb(132, 132, 132);\n"
"border-radius: 8px;\n"
"color: rgb(66, 255, 15);\n"
"padding-top: 5px;\n"
"padding-bottom: 5px;\n"
"padding-left: 8px;\n"
"padding-right: 4px;\n"
""));
        deviceInfo->setReadOnly(true);
        comboBox = new QComboBox(centralWidget);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 10, 231, 21));
        comboBox->setStyleSheet(QString::fromUtf8("QComboBox\n"
"{\n"
"border: 1px solid rgb(172, 169, 174);\n"
"            border-radius: 4px;\n"
"            background-color:rgb(42, 42, 42);\n"
"\n"
"            color: rgb(238, 238, 238);\n"
"}\n"
"\n"
"QComboBox:hover{\n"
"	background-color: rgb(81, 81, 81);\n"
"	color: rgb(23, 188, 255)\n"
"}\n"
"\n"
"QComboBox:disabled {\n"
"border-color: rgb(129, 128, 128);\n"
"background-color: rgb(80, 82, 82);\n"
"color: rgb(128, 130, 130);\n"
"}"));
        comboBox->setCurrentText(QString::fromUtf8("Level 1 - Basic"));
        comboBox->setIconSize(QSize(16, 16));
        MainWindow->setCentralWidget(centralWidget);
        frame_2->raise();
        frame->raise();
        rxButton->raise();
        txButton->raise();
        stopButton->raise();
        frame_3->raise();
        quitButton->raise();
        deviceButton->raise();
        deviceInfo->raise();
        comboBox->raise();
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setStyleSheet(QString::fromUtf8("background-color : rgb(131, 133, 139);"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(quitButton, SIGNAL(clicked()), MainWindow, SLOT(on_quitButton_clicked()));
        QObject::connect(gainSlider_RX1, SIGNAL(valueChanged(int)), MainWindow, SLOT(on_gainChanged_RX1()));
        QObject::connect(gainSlider_RX2, SIGNAL(valueChanged(int)), MainWindow, SLOT(on_gainChanged_RX2()));
        QObject::connect(gainSlider_TX1, SIGNAL(valueChanged(int)), MainWindow, SLOT(on_gainChanged_TX1()));
        QObject::connect(gainSlider_TX2, SIGNAL(valueChanged(int)), MainWindow, SLOT(on_gainChanged_TX2()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "BladeRF Explorer v0.00001", nullptr));
        rxButton->setText(QCoreApplication::translate("MainWindow", "RX START", nullptr));
        txButton->setText(QCoreApplication::translate("MainWindow", "TX START", nullptr));
        stopButton->setText(QCoreApplication::translate("MainWindow", "STOP STREAM", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Rx2 Gain", nullptr));
        gainValue_RX2->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "Tx1 Gain", nullptr));
        gainValue_TX1->setText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "Tx2 Gain", nullptr));
        gainValue_TX2->setText(QString());
        label_7->setText(QCoreApplication::translate("MainWindow", "Rx1 Gain", nullptr));
        gainValue_RX1->setText(QString());
        checkBox->setText(QCoreApplication::translate("MainWindow", "MIMO", nullptr));
        txFreqTextBox->setInputMask(QCoreApplication::translate("MainWindow", "0000000000", nullptr));
        txFreqTextBox->setText(QString());
        txFreqTextBox->setPlaceholderText(QString());
        rxFreqTextBox->setInputMask(QCoreApplication::translate("MainWindow", "0000000000", nullptr));
        rxFreqTextBox->setText(QString());
        rxFreqInput->setText(QCoreApplication::translate("MainWindow", "Rx FREQ", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Tx FREQ", nullptr));
        txFreqTextBox_2->setInputMask(QCoreApplication::translate("MainWindow", "0000000000", nullptr));
        txFreqTextBox_2->setText(QString());
        txFreqTextBox_2->setPlaceholderText(QString());
        rxFreqTextBox_2->setInputMask(QCoreApplication::translate("MainWindow", "0000000000", nullptr));
        rxFreqTextBox_2->setText(QString());
        rxFreqInput_2->setText(QCoreApplication::translate("MainWindow", "Rx FREQ", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Tx FREQ", nullptr));
        quitButton->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        deviceButton->setText(QCoreApplication::translate("MainWindow", "SHOW DEVICE INFO", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Level 1 - Basic", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Level 2 - Verbose", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Level 3 - Very Verbose", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Level 4 - Debug", nullptr));

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
