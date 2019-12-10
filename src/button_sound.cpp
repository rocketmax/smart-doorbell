#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include "../include/button_sound.hpp"

//setup sound

void buttonPress(){
    
  if (digitalRead(1) == 1){ //checks button press
    system("omxplayer ./ring.wav -o local" ); //makes system call to play door chime
    //display sound
  }

}

void buttonsound()
{
    
  for(;;) buttonPress(); //gadfly loop
}
