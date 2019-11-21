#include <thread>
#include <iostream>
#include <mutex>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat current;
int light, led;
mutex cmtx, lmtx, t;

void videoStream(){
  int deviceID = 0;
  int apiID = CAP_ANY;
  VideoCapture cap;
  cap.open(deviceID + apiID);
  cout << "Camera Feed Initialized" << endl;
  Mat frame, gray;
  for(;;){
    cap.read(frame);
    if (frame.empty()) {
      std::cerr << "ERROR: Blank frame grabbed\n";
      break;
    }
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    gray.convertTo(gray, CV_8UC1);
    cmtx.lock();
    current = gray;
    cmtx.unlock();
    //imshow("frame", current);
    //waitKey(5);
    //cout << "frame grabbed" << endl;
    t.unlock();
  }
}

void get_light(){
  int sum, avg;
  Mat temp;
  for(;;){
    cmtx.lock();
    temp = current;
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

void led_handler(){
  for(;;){
    if(light > 40 && led){
      cout << "LED OFF" << endl;
      led = 0;
    }
    else if(light < 40 && !led){
      cout << "LED ON" << endl;
      led = 1;
    }
  }
}


void print(int n){
  while(1) std::cout << n << std::endl;
}

int main(){

  thread streamT(videoStream);
  thread lightT(get_light);
  thread ledT(led_handler);


  streamT.join();
  lightT.join();
  ledT.join();

  return 0;
}
