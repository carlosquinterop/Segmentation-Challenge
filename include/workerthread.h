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

#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <qt5/QtCore/QObject>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <iostream>

using namespace cv;
using namespace std;

class workerThread : public QThread
{
    Q_OBJECT
    
    void run();
public:
    workerThread();
    void setWorkingThread(bool workThread);
    void setThreshold(int aLow_r, int aLow_g, int aLow_b, int aHigh_r, int aHigh_g, int aHigh_b);
    void setCameraId(int cameraId);
    
private:
    VideoCapture *stream1;
    bool work;
    int low_r, low_g, low_b;
    int high_r, high_g, high_b;
    int camId;
    
signals:
    void centerComputed(char instruction);
    
};

#endif // WORKERTHREAD_H