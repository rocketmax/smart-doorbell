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
#include <wiringPi.h>

#include "include/get_light.hpp"
#include "include/video_stream.hpp"
#include "include/led_handler.hpp"
#include "include/network_handler.hpp"
#include "include/motion_detect.hpp"
#include "include/button_sound.hpp"

using namespace std;
using namespace cv;

Mat current, past; //global images
int light, led, motion, margin; //global integers
float sensitivity; //global sensitivity for motion detecton
mutex cmtx, pmtx, mmtx, lmtx, t, n, d; //mutexes (semaphores)
time_t now;

int main(){

  margin = 5; //initializes motion detection parameters
  sensitivity = .9;
  
  wiringPiSetup(); //initializes GPIO
  pinMode(0, OUTPUT); // for LED
  pinMode(1, INPUT); //for button


  thread streamT(videoStream); //starts the camera thread
  thread lightT(get_light); //begins calculating the light in each image
  thread ledT(led_handler); //controls led state
  thread motionT(detect); //calculates if motion occurred
  thread butT(buttonsound); //polls button
  
  while(1){
    thread netT(network_handler); //continuously restarts server on client disconnect/thread completion
    netT.join();
  }

  streamT.join(); //waits for each thread to terminate before killing program (never)
  lightT.join();
  ledT.join();
  motionT.join();
  butT.join();

  return 0;
}
