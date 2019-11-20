#include <pthread.h>
#include <iostream>

void *thread1(void *arg){
  while(1) std::cout << '1' << std::endl;
}

void *thread2(void *arg){
  while(1) std::cout << '2' << std::endl;
}

void *thread3(void *arg){
  while(1) std::cout << '3' << std::endl;
}

int main(){
  //std::cout << "helpme" << std::endl;
  pthread_t t1, t2, t3;
  int ce1 = pthread_create(&t1, NULL, thread1, NULL);
  int ce2 = pthread_create(&t2, NULL, thread2, NULL);
  int ce3 = pthread_create(&t3, NULL, thread3, NULL);
  while(1);
  return 0;
}
