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
    current.copyTo(temp); //grabs most recent frame
    cmtx.unlock();
    if(!temp.empty()){
      sum = cv::sum(temp)[0]; //gets sum of all greyscale pixels in the single channel
      avg = sum / (temp.rows*temp.cols); //avgs
      lmtx.lock();
      light = avg; //stores avg light level
      lmtx.unlock();
      //cout << "Light: " << avg << endl;
    }
    t.lock(); //wait for new frame
  }
}
