#include <iostream>
#include <mutex>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../include/motion_detect.hpp"

using namespace std;

void detect() {
  int count, pixels;
  float change;
  cv::Mat lowerB, upperB, dst;
  cv::Mat c, p;
  motion = 0;
  int counter;

  for(;;){
    cmtx.lock();
    current.copyTo(c); //get current frame
    cmtx.unlock();
    pmtx.lock();
    past.copyTo(p); //get last frame
    pmtx.unlock();
    if(c.empty() || p.empty()) continue; //validity check

    count = 0;
    pixels = c.size().height * c.size().width;
    lowerB = p - 5; //generate lower/upper bounds of what pixels are considered unchanged
    upperB = p + 5;
    cv::inRange(current, lowerB, upperB, dst); //generate a binary image to display which pixels changed beyond acceptable margin
    change = (float) cv::countNonZero(dst) / pixels; //get percentage of pixels changed
    //cv::imshow("test", dst); //display change
    //std::cout << count << '/' << pixels << '=' << change << std::endl;
    //cout << counter << '\t';
    if (change < sensitivity){ //higher for 
      if(!motion) cout << "Motion began" << endl;
      mmtx.lock();
      motion = 1;
      mmtx.unlock();
      counter = 40;
    }  
    else {
      if(counter == 0) {
        if(motion) cout << "Motion ended" << endl;
        mmtx.lock();
        motion = 0;
        mmtx.unlock();
      }
      else counter--;
    }
    d.lock();
  }
}
