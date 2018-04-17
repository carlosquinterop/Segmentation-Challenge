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

#include "../include/workerthread.h"

workerThread::workerThread()
{
  work = false;  
  camId = 0;
}

void workerThread::setWorkingThread(bool workThread)
{
    work = workThread;
}


void workerThread::run()
{
    Mat capturedImage;
    stream1 = new VideoCapture(camId);
    char instruction = 0;
        
    while(work)
    {
	*stream1 >> capturedImage;
	emit centerComputed(instruction);
	imshow("Real time video", capturedImage);
    }
    delete stream1;
}

void workerThread::setThreshold(int aLow_r, int aLow_g, int aLow_b, int aHigh_r, int aHigh_g, int aHigh_b)
{
    low_g = aLow_g;
    low_b = aLow_b;
    low_r = aLow_r;
    
    high_g = aHigh_g;
    high_b = aHigh_b;
    high_r = aHigh_r;
}

void workerThread::setCameraId(int cameraId)
{
    camId = cameraId;
}