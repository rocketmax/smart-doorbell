#include <iostream>
#include <wiringPi.h>

#include "../include/led_handler.hpp"

void led_handler(){
  wiringPiSetup();
  for(;;){
    /*if(light > 40 && led){
      std::cout << "LED OFF" << std::endl;
      led = 0;
    }
    else if(light < 40 && !led){
      std::cout << "LED ON" << std::endl;
      led = 1;
    }*/
    if (light <= 40 && !led){
      digitalWrite(0,1);
      std::cout << "LED ON" << std::endl;
      led = 1;
    }
    else if (light >= 60 && led){
      digitalWrite(0, 0);
      std::cout << "LED OFF" << std::endl;
      led = 0;
    }
  }
}
