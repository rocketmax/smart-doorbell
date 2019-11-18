#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>

#define PORT 8080

using namespace std;
using namespace cv;

/*vector<uchar> mat2vec(Mat mat){

  std::vector<uchar> array;
  if (mat.isContinuous())
    array.assign(mat.data, mat.data + mat.total());
  else
    for (int i = 0; i < mat.rows; ++i)
      array.insert(array.end(), mat.ptr<uchar>(i), mat.ptr<uchar>(i)+mat.cols);
  return array;
}*/

vector<vector<uchar>> mat2vecs(Mat mat){

  Mat row;
  vector<uchar> temp;
  vector<vector<uchar>> array;

  for (int i = 0; i < mat.rows; ++i){
    row = mat.row(i);
    temp.clear();
    temp.insert(temp.end(), row.ptr<uchar>(0), row.ptr<uchar>(0)+row.cols);
    array.push_back(temp);
  }

  cout << array.size() << '\t' << array[0].size() << endl;
  return array;
}

/*void printV(vector<uchar> v){

  vector<uchar>::iterator p;
  for(p = v.begin(); p < v.end(); p++)
    cout << int(*p) << '\t';
  cout << endl;
}

void printVs(vector<vector<uchar>> v){

  vector<vector<uchar>>::iterator ptr;
  for(ptr = v.begin(); ptr < v.end(); ++ptr)
    printV(*ptr);
}*/

/*Mat vecs2mat(vector<vector<uchar>> v){

  Mat image(0, v[0].size(), CV_8UC1);

  for(int i = 0; i < v.size(); ++i){
    Mat sample(1, v[0].size(), CV_8UC1, v[i].data());
    image.push_back(sample);
  }
  return image;
}*/

int main(int argc, char *argv[]){

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    Mat bwi, image = imread("/home/ubuntafoo/Downloads/dog.jpg", CV_LOAD_IMAGE_COLOR);
    cvtColor(image, bwi, COLOR_BGR2GRAY);
    bwi.convertTo(bwi, CV_8UC1);
    resize(bwi, bwi, Size(), 0.5, 0.5, INTER_LINEAR);
    vector<vector<uchar>> iv = mat2vecs(bwi);
    //vector<uchar> test;
    //imencode("jpeg", bwi, test, NULL);

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

    char sz[10];
    sprintf( sz, "%d,%d", bwi.rows, bwi.cols);
    cout << sz << endl;
    //sprintf( w, "%d", bwi.cols);
    send(new_socket, sz, strlen(sz), 0);

    //send(new_socket, test.data(), test.size(), 0);
    for(int i = 0; i < bwi.rows; i ++){
      send(new_socket, iv[i].data(), bwi.cols, 0);
      usleep(500);
      //valread = read( new_socket, buffer, 1);
      //if(buffer[0] != 'a') {
      //  cout << "Error detected" << endl;
      //  break;
      //}
    }

    //send(new_socket, w, strlen(w), 0);
    //send(new_socket , hello , strlen(hello) , 0 );
    //send(new_socket, reinterpret_cast<char*>(a), image.rows*image.cols, 0);
    //printf("Hello message sent\n");
    return 0;
}

