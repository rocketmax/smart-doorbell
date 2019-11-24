#include <iostream>

#include "../include/led_handler.hpp"

void led_handler(){
  for(;;){
    if(light > 40 && led){
      std::cout << "LED OFF" << std::endl;
      led = 0;
    }
    else if(light < 40 && !led){
      std::cout << "LED ON" << std::endl;
      led = 1;
    }
  }
}