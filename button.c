// compilieren mit g++ -std=c++0x button.c -o button -lwiringPi
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <string>
#include <string.h>
#include "button.h"
#include "sharedMemory.h"

using namespace std;

char bUploadState, bStartState;

void initButton(){
   // set pin directions
  pinMode(bUpload, INPUT);
  pinMode(bStart , INPUT);
  bUploadState = 0;
  bStartState = 0;
}

char UploadButtonPressed(){
  if( !bUploadState && (bUploadState = digitalRead(bUpload)) )
    return 1;
  else
    bUploadState = digitalRead(bUpload);
  return 0; 
}   

char StartButtonPressed(){
  if( !bStartState  && (bStartState  = digitalRead(bStart)) )
    return 1;
  else
    bStartState  = digitalRead(bStart);
  return 0;
}
  

int main(int argc, char** argv){

  int shID;
  SM *SMPtr;

  char StartButtonClicked = 0;
  char UploadButtonClicked = 0;
  
  wiringPiSetup();
  initButton();

  while(1){

    // allocate memory
    shID = shmget(2404, MAXSM, 0666); 
    SMPtr = (SM*)shmat(shID, 0, 0); 
  
    if( UploadButtonPressed() ) SMPtr->UploadButtonPressed = 1;
    if( StartButtonPressed()  ) SMPtr->StartButtonPressed  = 1;
     
    // deallocate memory
    shmdt(SMPtr); 
    usleep(1000);
  }
  


}
