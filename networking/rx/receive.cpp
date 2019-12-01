#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;
using namespace cv;

#define PORT 8080

void printV(vector<uchar> v){

  vector<uchar>::iterator p;
  for(p = v.begin(); p < v.end(); p++)
    cout << int(*p) << '\t';
  cout << endl;
}

void printV(vector<uchar> v, int len){
  if(v.size() < len) len = v.size();
  for(int i = 0; i < len; ++i){
    cout << int(v[i]) << '\t';
  }
  cout << endl;
}

Mat vecs2mat(vector<vector<uchar>> v){

  Mat image(0, v[0].size(), CV_8UC1);

  for(int i = 0; i < v.size(); ++i){
    Mat sample(1, v[0].size(), CV_8UC1, v[i].data());
    image.push_back(sample);
  }
  return image;
}

int main(int argc, char const *argv[])
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Client connected";
    char *ack = "a";
    char *err = "e";
    char sz_buf[10] = {0};
    char buffer[40000] = {0};

    uchar * a;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    //if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) //localhost
    //if(inet_pton(AF_INET, "10.4.5.21", &serv_addr.sin_addr)<=0) //union
    //if(inet_pton(AF_INET, "141.215.216.195", &serv_addr.sin_addr)<=0) //school
    //if(inet_pton(AF_INET, "192.168.1.72", &serv_addr.sin_addr)<=0) //home
    if(inet_pton(AF_INET, "192.168.43.50", &serv_addr.sin_addr)<=0) //hotspot
    //if(inet_pton(AF_INET, "32.7.252.160", &serv_addr.sin_addr)<=0) //MGuest
    //if(inet_pton(AF_INET, "10.4.2.92", &serv_addr.sin_addr)<=0) //rpi
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Connected to Server\n");

    int siz;
    vector<vector<uchar>> im;

    vector<uchar> rx;
    Mat image;

    while(1){
      valread = read(sock, sz_buf, 10);
      //cout << "Expecting " << sz_buf << endl;
      siz = atoi(sz_buf);
      rx.clear();

      for(int i = 0; i < siz;i+=valread){
        valread = recv(sock, buffer, siz - i, 0);
        vector<uchar> temp(buffer, buffer+valread);
        rx.insert(rx.end(), temp.begin(), temp.end());
      }
      //while(siz < rx.size()) rx.pop_back();
      //cout << "Received " << rx.size() << endl;
      image = imdecode(rx, 1);
      if(!image.empty()) imshow("image", image);
      if(waitKey(1) == 'q') break;
      send(sock, ack, 2, 0);
    }

    return 0;
}

