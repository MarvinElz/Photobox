// compilieren mit g++ -std=c++0x button.c -o button -lwiringPi
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <string>
#include "button.h"

using namespace std;

int counter = 1;

int main(int argc, char** argv){
  cout << "Hallo Welt" << endl;
  int bUploadState, bStartState;
  bUploadState = 0;
  bStartState = 0;
  wiringPiSetup();

  // set pin directions
  pinMode(bUpload, INPUT);
  pinMode(bStart , INPUT);

  while(1){
   
    if( !bUploadState && (bUploadState = digitalRead(bUpload)) )
      cout << "bUpload pressed" << endl;
    else
      bUploadState = digitalRead(bUpload);
 
    if( !bStartState  && (bStartState  = digitalRead(bStart)) )
      cout << "bStart pressed" << endl;
    else
      bStartState  = digitalRead(bStart);

    usleep(1000);
  }

  
   

}
