// motion_detect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <mutex>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../include/get_light.hpp"

void get_light(){
  int sum, avg;
  cv::Mat temp;
  for(;;){
    cmtx.lock();
    current.copyTo(temp);
    cmtx.unlock();
    if(!temp.empty()){
      sum = cv::sum(temp)[0];
      avg = sum / (temp.rows*temp.cols);
      lmtx.lock();
      light = avg;
      lmtx.unlock();
      //cout << "Light: " << avg << endl;
    }
    t.lock();
  }
}