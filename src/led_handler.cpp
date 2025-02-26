#include <iostream>
#include <wiringPi.h>

#include "../include/led_handler.hpp"

void led_handler(){

  for(;;){
    if (light <= 40 && !led){ //threshold to turn LED on
      digitalWrite(0,1);
      std::cout << "LED ON" << std::endl;
      led = 1;
    }
    else if (light >= 60 && led){ //threshold to turn LED off
      digitalWrite(0, 0);
      std::cout << "LED OFF" << std::endl;
      led = 0;
    }
  }
}
