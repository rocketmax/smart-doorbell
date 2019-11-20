// motion_detect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int get_light(cv::Mat img){
  int sum = cv::sum(img)[0];
  sum /= img.rows*img.cols;
  return sum;
}

int desiredFPS(float processTime, int fps) {

  int wait;
  wait = int((1 - processTime) * 1000 / fps);
  if (fps == -1)
    return 5;
  return wait;
}

int main(int argc, char *argv[]) {
  //std::cout << "Hello World!\n";
  const cv::String keys = 
      "{help h usage ?|     | print this message      }"
      "{fps           | -1  | framerate cap (def: inf)}"
      "{device        | 0   | webcam device number    }"
      ;
  cv::CommandLineParser parser(argc, argv, keys);
  parser.about("motion_detector for smart-doorbell");
  if (parser.has("help")){
    parser.printMessage();
    return 0;
  }

  clock_t start;
  int targetFPS = parser.get<int>("fps"); //-1 = no limit
  float lap, fps;
  cv::Mat frame, greyFrame;
  cv::VideoCapture cap;
  int deviceID = parser.get<int>("device");
  int apiID = cv::CAP_ANY;
  cap.open(deviceID + apiID);
  int light;

  if (!cap.isOpened()) {
    std::cerr << "ERROR: Unable to open camera\n";
    return -1;
  }

  std::cout << "Start grabbing" << std::endl << "Press any key to quit" << std::endl;
  cv::namedWindow("Live");

  for (;;) {
    start = clock();
    cap.read(frame);
    if (frame.empty()) {
      std::cerr << "ERROR: Blank frame grabbed\n";
      break;
    }
    cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
    greyFrame.convertTo(greyFrame, CV_8UC1);
    light = get_light(greyFrame);
    std::cout << light << std::endl;
    cv::imshow("Live", greyFrame);
    lap = float(clock() - start) / CLOCKS_PER_SEC;
    cv::waitKey(desiredFPS(lap, targetFPS));
    fps = CLOCKS_PER_SEC / float(clock() - start);
    std::cout << "\t FPS: " << fps << std::endl;
  }
}
