// motion_detect.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

bool detect(cv::Mat current, cv::Mat past, float sensitivity) {

  int count = 0;
  int margin = 3;
  int pixels = current.size().height * current.size().width;
  cv::Mat lowerB, upperB, dst;
  lowerB = past - 5;
  upperB = past + 5;
  cv::inRange(current, lowerB, upperB, dst);
  float change = (float) cv::countNonZero(dst) / pixels;
  cv::imshow("test", dst);
  //std::cout << count << '/' << pixels << '=' << change << std::endl;
  if (change < sensitivity) //higher for
    return true;
  else
    return false;
}

int desiredFPS(float processTime, int fps) {

  int wait;
  wait = int((1 - processTime) * 1000 / fps);
  if (fps == -1)
    return 5;
  return wait;
}

int main() {
  //std::cout << "Hello World!\n";
  clock_t start;
  int wait = 1;
  int targetFPS = 22; //-1 = no limit
  float sensitivity = 0.995; //higher for more sens, works better at distance + in dark. lower gives less false positives
                            //could make dynamic by sensing light level, maybe average pixel value to alter
  float lap, fps;
  cv::Mat frame, greyFrame, lastFrame;
  cv::VideoCapture cap;
  int deviceID = 0;
  int apiID = cv::CAP_ANY;
  cap.open(deviceID + apiID);
  cv::VideoWriter writer;
  std::string name;
  int vidcount = 0;
  int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
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
    if (!lastFrame.empty())
    if (detect(greyFrame, lastFrame, sensitivity)) {
      std::cout << "MOTION DETECTED";
      name = "./camera" + std::to_string(vidcount) + ".avi";
      if (!writer.isOpened()) {
        if(targetFPS == -1) writer.open(name, codec, 22, frame.size(), true);
        else writer.open(name, codec, targetFPS, frame.size(), true);
      }
      writer.write(frame);
    }
    else {
      std::cout << "no motion";
      /*if (writer.isOpened()) {  //uncomment to create new video for each motion, comment to make all motion one video
        writer.release();
        vidcount++;
      }*/
    }
    greyFrame.copyTo(lastFrame);
    cv::imshow("Live", greyFrame);
    lap = float(clock() - start) / CLOCKS_PER_SEC;
    //if (cv::waitKey(desiredFPS(lap, targetFPS)) != -1)
    //  break;
    cv::waitKey(desiredFPS(lap, targetFPS));
    fps = CLOCKS_PER_SEC / float(clock() - start);
    std::cout << "\t FPS: " << fps << std::endl;
  }
}
