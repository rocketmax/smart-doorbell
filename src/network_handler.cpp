#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>

#include "../include/network_handler.hpp"

using namespace std;
using namespace cv;

void network_handler(){

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char status[2] = "a";
    int PORT = 8080;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    Mat bwi, frame;
    vector<uchar> iv;
    vector<int> params = vector<int>(2);
    params[0] = 1;
    params[1] = 90;

    char sz[10], sz_back[10];

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    while(status[0] == 'a'){
      cmtx.lock();
      frame = current;
      cmtx.unlock();

      if(frame.empty()) continue;

      iv.clear();
      imencode(".jpg", frame, iv, params);
      //printV(iv, 100);

      //cout << "Sending " << iv.size() << endl;
      sprintf(sz, "%d", iv.size());
      send(new_socket, sz, 10, 0); //sends compressed img size
      //status[0] = 'e';
      //valread = read(new_socket, status, 2);

      if(status[0] != 'a') continue;
      send(new_socket, iv.data(), iv.size(), 0); //sends compressed img
      status[0] = 'e'; //assume err if nothing is received in the next line
      valread = read(new_socket, status, 2); //waits for ack or err

      n.lock(); //wait for new frame before sending another
    }  

    cout << "Client disconnected" << endl;
    close(new_socket);
}