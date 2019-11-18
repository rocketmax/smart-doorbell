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
    char sz_buf[10] = {0};
    char buffer[1920] = {0};
    //Mat image;
    //vector<uchar> iv;
    //iv.reserve(1024);
    //vector<uchar> *ptr = &iv;

    uchar * a;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    //if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    if(inet_pton(AF_INET, "10.4.5.21", &serv_addr.sin_addr)<=0)
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
    //valread = read( sock , ptr, 1024);
    valread = read( sock, sz_buf, 1920);
    char * height = strtok (sz_buf,",");
    char * width = strtok (NULL, ",");
    int h = atoi(height);
    int w = atoi(width);
    //valread = read(sock, buffer, 1920);
    //int w = atoi(buffer);
    cout << h << '\t' << w << endl;
    vector<vector<uchar>> im;

    for(int i = 0; i < h; i++){
      valread = read(sock, buffer, w);
      vector<uchar> temp(buffer, buffer+w);
      im.push_back(temp);
      //send(sock, ack, strlen(ack), 0);
    }

    vector<uchar> rx;

    /*while(valread){
      valread = read(sock, buffer, 1920);
      vector<uchar> temp(buffer, buffer+1920);
      rx.insert(rx.end(), temp.begin(), temp.end());
    }

    Mat image = imdecode(rx, 0);*/

    //a = reinterpret_cast<uchar*>(buffer);

    Mat image = vecs2mat(im);
    //printf("%s\n",buffer );
    //for(int i = 0; i < 81000; i++)
    //  cout << int(a[i]) << '\t';
    imshow("image", image);
    //printV(iv);
    waitKey(0);
    return 0;
}

