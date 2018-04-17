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

#ifndef openCVStudents_H
#define openCVStudents_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QtSerialPort/QSerialPort>
#include <QtWidgets/QMessageBox>
#include <opencv2/opencv.hpp>
#include <QDirIterator>
#include "include/workerthread.h"

using namespace std;
using namespace cv;

class openCVStudents : public QMainWindow
{
    Q_OBJECT

private:
    QGridLayout *layout;
    QLineEdit *filePath;
    QLineEdit *redLow, *redHigh, *greenLow, *greenHigh, *blueLow, *blueHigh;
    QPushButton *startButton;
    QPushButton *captureButton;
    QPushButton *stopCaptureButton;
    QPushButton *startSerialConnection;
    QPushButton *stopSerialConnection;
    QLabel *showImageLabel;
    QLabel *captureImageLabel;
    QComboBox *camId;
    workerThread *myThread;
    int low_r, high_r, low_g, high_g, low_b, high_b;
    bool serialConnectionAllowed;
    QSerialPort *serialPort;
    bool serialConnected;
    
public:
    openCVStudents();
    virtual ~openCVStudents();
    
    
private slots:
    void showImage();
    void captureImage();
    void stopCapturingImage();
    void openSerialPort();
    void closeSerialPort();
    void redLowUpdate(QString text);
    void redHighUpdate(QString text);
    void greenLowUpdate(QString text);
    void greenHighUpdate(QString text);
    void blueLowUpdate(QString text);
    void blueHighUpdate(QString text);
    void sendSerialMessage(char instruction);
};

#endif // openCVStudents_H
