#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <time.h>
#include <ctime>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>

#include "include/get_light.hpp"
#include "include/video_stream.hpp"
#include "include/led_handler.hpp"
#include "include/network_handler.hpp"
#include "include/motion_detect.hpp"

using namespace std;
using namespace cv;

Mat current, past;
int light, led, motion, margin;
float sensitivity;
mutex cmtx, pmtx, mmtx, lmtx, t, n, d;
time_t now;

int main(){

  margin = 5;
  sensitivity = .9;

  thread streamT(videoStream);
  thread lightT(get_light);
  thread ledT(led_handler);
  thread motionT(detect);
  
  while(1){
    thread netT(network_handler);
    netT.join();
  }

  streamT.join();
  lightT.join();
  ledT.join();
  motionT.join();

  return 0;
}
