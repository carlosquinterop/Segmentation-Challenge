/*
 * Copyright 2018 Carlos Quintero <quintero.carlos@uniandes.edu.co>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "include/openCVStudents.h"

openCVStudents::openCVStudents()
{    
    
    low_r = 120;
    low_g = 0;
    low_b = 0;
    high_r = 255;
    high_g = 70;
    high_b = 70;
    
    layout = new QGridLayout();
    filePath = new QLineEdit("");
    startButton = new QPushButton("Show");
    captureButton = new QPushButton("Capture");
    stopCaptureButton = new QPushButton("Stop");
    startSerialConnection = new QPushButton("Start Connection");
    stopSerialConnection = new QPushButton("Stop Connection");
    stopSerialConnection->setEnabled(false);
    stopCaptureButton->setEnabled(false);
    camId = new QComboBox();
    showImageLabel = new QLabel("Show image from file");
    captureImageLabel = new QLabel("Capture image from camera");
    redLow = new QLineEdit(QString::number(low_r));
    redHigh = new QLineEdit(QString::number(high_r));
    greenLow = new QLineEdit(QString::number(low_g));
    greenHigh = new QLineEdit(QString::number(high_g));
    blueLow = new QLineEdit(QString::number(low_b));
    blueHigh = new QLineEdit(QString::number(high_b));
    
    layout->addWidget(showImageLabel, 0, 0);
    layout->addWidget(filePath, 0, 1);
    layout->addWidget(startButton, 0, 2);
    layout->addWidget(new QLabel("Camera Id"), 1, 0);
    layout->addWidget(camId, 1, 1);
    layout->addWidget(captureImageLabel, 2, 0);
    layout->addWidget(captureButton, 2, 1);
    layout->addWidget(stopCaptureButton, 2, 2);
    layout->addWidget(new QLabel("RGB Threshold parameters"), 3, 0);
    layout->addWidget(new QLabel("R low"), 4, 0);
    layout->addWidget(redLow, 4, 1);
    layout->addWidget(new QLabel("R high"), 4, 2);
    layout->addWidget(redHigh, 4, 3);
    layout->addWidget(new QLabel("G low"), 5, 0);
    layout->addWidget(greenLow, 5, 1);
    layout->addWidget(new QLabel("G high"), 5, 2);
    layout->addWidget(greenHigh, 5, 3);
    layout->addWidget(new QLabel("B low"), 6, 0);
    layout->addWidget(blueLow, 6, 1);
    layout->addWidget(new QLabel("B high"), 6, 2);
    layout->addWidget(blueHigh, 6, 3);
    layout->addWidget(new QLabel("Serial Communication"), 7, 0);
    layout->addWidget(startSerialConnection, 7, 1);
    layout->addWidget(stopSerialConnection, 7, 2);
    QWidget *window = new QWidget();
    window->setLayout(layout);
    setCentralWidget(window);    
            
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(showImage()));
    connect(captureButton, SIGNAL(clicked(bool)), this, SLOT(captureImage()));
    connect(stopCaptureButton, SIGNAL(clicked(bool)), this, SLOT(stopCapturingImage()));
    connect(startSerialConnection, SIGNAL(clicked(bool)), this, SLOT(openSerialPort()));
    connect(stopSerialConnection, SIGNAL(clicked(bool)), this, SLOT(closeSerialPort()));
    connect(redLow, SIGNAL(textEdited(QString)), this, SLOT(redLowUpdate(QString)));
    connect(redHigh, SIGNAL(textEdited(QString)), this, SLOT(redHighUpdate(QString)));
    connect(greenLow, SIGNAL(textEdited(QString)), this, SLOT(greenLowUpdate(QString)));
    connect(greenHigh, SIGNAL(textEdited(QString)), this, SLOT(greenHighUpdate(QString)));
    connect(blueLow, SIGNAL(textEdited(QString)), this, SLOT(blueLowUpdate(QString)));
    connect(blueHigh, SIGNAL(textEdited(QString)), this, SLOT(blueHighUpdate(QString)));
    myThread = new workerThread();
    myThread->setThreshold(low_r, low_g, low_b, high_r, high_g, high_b);
    connect(myThread, SIGNAL(centerComputed(char)), this, SLOT(sendSerialMessage(char)));
    serialConnectionAllowed = false;
    serialConnected = false;
    
    int camCont = 0;
    QDirIterator it("/dev", QDirIterator::NoIteratorFlags);
    QDir dir("/dev");
    bool exists = false;
    do
    {
	QString file = "video" + QString::number(camCont);
	if (dir.exists(file))
	{
	    camCont++;
	    exists = true;
	}
	else
	{
	    exists = false;   
	}
    }
    while(exists);
    for(int i = 0; i < camCont;i++)
	camId->addItem(QString::number(i));
    
    camId->setCurrentIndex(0);
}

openCVStudents::~openCVStudents()
{
    myThread->exit();
}

void openCVStudents::openSerialPort()
{
    if (serialConnectionAllowed)
    {
	serialPort = new QSerialPort();
	serialPort->setPortName("ttyACM0");//For Arduino
	serialPort->setBaudRate(QSerialPort::Baud115200);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setStopBits(QSerialPort::OneStop);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);
	
	if(serialPort->open(QIODevice::ReadWrite)) 
	{
	    stopSerialConnection->setEnabled(true);
	    startSerialConnection->setEnabled(false);
	    serialConnected = true;
	} 
	else 
	{
	    QMessageBox msgBox;
	    msgBox.setText(QString("Error openning ttyUSB0"));
	    msgBox.setIcon(QMessageBox::Critical);
	    msgBox.exec();
	}
    }
    else
    {
	QMessageBox msgBox;
	msgBox.setText(QString("Enable camera capture first"));
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.exec();
    }
}

void openCVStudents::closeSerialPort()
{
    if(serialPort->isOpen())
    {
	serialPort->close();
	stopSerialConnection->setEnabled(false);
	startSerialConnection->setEnabled(true);
	serialConnected = false;
    }
}

void openCVStudents::sendSerialMessage(char instruction)
{
    if (serialConnected)
    {
	const char* data = &instruction;
	if(serialPort->isOpen())
	    serialPort->write(data);
    }
}

void openCVStudents::showImage()
{
    Mat image;
    QString imagePath = filePath->text();
    image = imread( imagePath.toStdString(), 1 );

    if ( !image.data )
    {
         QMessageBox msgBox;
	 msgBox.setText(QString("Error openning %1").arg(filePath->text()));
	 msgBox.setIcon(QMessageBox::Critical);
	 msgBox.exec();
    }
    else
    {
	namedWindow("Display Image", WINDOW_AUTOSIZE );
	imshow("Display Image", image);
    }
}

void openCVStudents::captureImage()
{
    myThread->setWorkingThread(true);
    myThread->setCameraId(camId->currentIndex());
    myThread->start();
    stopCaptureButton->setEnabled(true);
    captureButton->setEnabled(false);
    namedWindow("Real time video", WINDOW_AUTOSIZE );
    serialConnectionAllowed = true;
}

void openCVStudents::stopCapturingImage()
{
    myThread->setWorkingThread(false);
    stopCaptureButton->setEnabled(false);
    captureButton->setEnabled(true);
    destroyWindow("Real time video");
    serialConnectionAllowed = false;
    if (serialConnected)
	closeSerialPort();
}

void openCVStudents::redLowUpdate(QString text)
{
    low_r = text.toInt();
    myThread->setThreshold(low_r, low_g, low_b, high_r, high_g, high_b);
}

void openCVStudents::redHighUpdate(QString text)
{
    high_r = text.toInt();
    myThread->setThreshold(low_r, low_g, low_b, high_r, high_g, high_b);
}

void openCVStudents::greenLowUpdate(QString text)
{
    low_g = text.toInt();
    myThread->setThreshold(low_r, low_g, low_b, high_r, high_g, high_b);
}

void openCVStudents::greenHighUpdate(QString text)
{
    high_g = text.toInt();
    myThread->setThreshold(low_r, low_g, low_b, high_r, high_g, high_b);
}

void openCVStudents::blueLowUpdate(QString text)
{
    low_b = text.toInt();
    myThread->setThreshold(low_r, low_g, low_b, high_r, high_g, high_b);
}

void openCVStudents::blueHighUpdate(QString text)
{
    high_b = text.toInt();
    myThread->setThreshold(low_r, low_g, low_b, high_r, high_g, high_b);
}
