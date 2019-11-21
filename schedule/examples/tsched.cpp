#include <thread>
#include <iostream>

using namespace std;

void print(int n){
  while(1) std::cout << n << std::endl;
}

int main(){

  thread th1(print, 1);
  thread th2(print, 2);
  thread th3(print, 3);

  th1.join();
  th2.join();
  th3.join();
  return 0;
}
