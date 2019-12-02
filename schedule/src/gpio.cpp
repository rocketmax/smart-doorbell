#include <iostream>
#include <wiringPi.h>

#include "../include/gpio.hpp"

using namespace std;

void buttonPress(){
  if (digitalRead(1) == 1){
//play sound
  }
}

void lightsensor(int lightvalue){
  if (lightvalue <= 40 && digitalRead(0) == 0){
    digitalWrite(0,1);
  }
  if (lightvalue >= 60 && digitalRead(0) == 1){
    digitalWrite(0, 0);
  }
}
