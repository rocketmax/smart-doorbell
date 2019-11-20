#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

void wait(int seconds){
  boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void thread(){
  for(int i = 0; i < 5; ++i){
    //wait(1);
    std::cout << i << '\n';
  }
}

void thread1(){
  while(1) std::cout << '1' << std::endl;
}

void thread2(){
  while(1) std::cout << '2' << std::endl;
}

void thread3(){
  while(1) std::cout << '3' << std::endl;
}

int main(){
  //std::cout << "helpme" << std::endl;
  //boost::thread t{thread};
  boost::thread t1{thread1};
  boost::thread t2{thread2};
  boost::thread t3{thread3};
  while(1);
  return 0;
}
