Smart Doorbell
===
Introduction
---
This project was created for ECE 473 under Professor Shaout and UM-Dearborn. It is a multithreaded C++ application to be run as 
embedded software on a Raspberry Pi that utilizes OpenCV and acts as a doorbell that can:
* Ring when a button is pressed
* Stream video over WiFi using sockets to another UNIX-like device
* Turn a light on when it is dark outside
* Detect motion and save a quick video when motion is detected
* Save the last 24 hours of video all the time

Building
---
0. Make sure all requirements are satisfied:
  * [Raspberry Pi OS: Raspbian](https://www.raspberrypi.org/downloads/)
  * [OpenCV](https://opencv.org/) `sudo apt install libopencv-dev`
  * [CMake](https://cmake.org/) `sudo apt install cmake`
1. Clone the project `git clone github.com/rocketmax/smart-doorbell`
2. Open the project `cd smart-doorbell`
3. Prepare CMake `cmake .`
4. Build the project `make`

Running
---
Run using `./doorbell`
