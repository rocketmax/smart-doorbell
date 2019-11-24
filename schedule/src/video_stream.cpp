#include <iostream>
#include <mutex>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../include/video_stream.hpp"

using namespace cv;
using namespace std;

void videoStream(){
  int deviceID = 0;
  int apiID = CAP_ANY;
  cv::VideoCapture cap;
  cap.open(deviceID + apiID);
  cout << "Camera Feed Initialized" << endl;
  cv::Mat frame, gray;
  for(;;){
    cap.read(frame);
    if (frame.empty()) {
      std::cerr << "ERROR: Blank frame grabbed\n";
      break;
    }
    cv::cvtColor(frame, gray, COLOR_BGR2GRAY);
    gray.convertTo(gray, CV_8UC1);
    cmtx.lock();
    current = gray;
    cmtx.unlock();
    //imshow("frame", current);
    //waitKey(5);
    //cout << "frame grabbed" << endl;
    t.unlock();
    n.unlock();
  }
}