#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include "../include/button_sound.hpp"

//setup sound

void buttonPress(){
    
  if (digitalRead(1) == 1){
    system("omxplayer ./ring.wav -o local" );
    //display sound
  }

}

void buttonsound()
{
    
  for(;;) buttonPress();
}
