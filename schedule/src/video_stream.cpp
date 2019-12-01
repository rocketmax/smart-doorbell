#include <iostream>
#include <ctime>
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

  cv::VideoWriter writer, mwriter;
  int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');

  cv::Mat frame, gray;
  char * strtime;
  tm * datetime;
  int hour, lastmotion = 0; 
  string name, mname;

  for(;;){
    cap.read(frame);
    if (frame.empty()) {
      std::cerr << "ERROR: Blank frame grabbed\n";
      break;
    }
    cv::cvtColor(frame, gray, COLOR_BGR2GRAY);
    gray.convertTo(gray, CV_8UC1);
    pmtx.lock();
    current.copyTo(past);
    pmtx.unlock();
    cmtx.lock();
    gray.copyTo(current);
    cmtx.unlock();

    now = time(0);
    strtime = ctime(&now);
    datetime = localtime(&now);

    cv::putText(frame, strtime, {frame.rows - 180, 50}, FONT_HERSHEY_SIMPLEX, 1, {255,255,255});

    if(hour != datetime->tm_hour || !writer.isOpened()){
      name = "recordings/all/" + to_string(datetime->tm_hour) + "00record.mp4";
      writer.open(name, codec, 22, frame.size(), true);
      cout << "New recording started: " << name << endl;
    }

    if(!lastmotion && motion){
      mname = "recordings/" + to_string(datetime->tm_mon + 1) + "-" +
        to_string(datetime->tm_mday) + "," +
        to_string(datetime->tm_hour) + ":" +
        to_string(datetime->tm_min) + ":" +
        to_string(datetime->tm_sec) + ".mp4";
      mwriter.open(mname, codec, 22, frame.size(), true);
      cout << "New motion recording started: " << mname << endl;
    }

    lastmotion = motion;

    //imshow("frame", frame);
    //waitKey(5); 
    hour = datetime->tm_hour;
    writer.write(frame);
    if(motion) mwriter.write(frame);

    //cout << "frame grabbed" << endl;
    t.unlock();
    n.unlock();
    d.unlock();
  }
}
