// compilieren mit g++ -std=c++0x main.c -o main -lwiringPi
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <string>
#include <string.h>
#include "sharedMemory.h"


using namespace std;

string getTerminalOutput( string cmd );

int shID;
SM *SMPtr;
SM localSM = {255,0,0,0};
int i;
int counter = 1;


string shoot[] = {"sudo gphoto2 --capture-image-and-download --keep --interval 4 --frames 4 --force-overwrite --filename \"raw/", 
		  "_%01n.jpg\" &"};

int main(int argc, char** argv){
  cout << "Hallo Welt" << endl;

//  initialization  *************************
  // create shared memory
  shID = shmget(2404, MAXSM, IPC_CREAT | 0666);
  SMPtr = (SM*)shmat(shID, 0, 0);
  memcpy(SMPtr, &localSM, MAXSM);
  shmdt(SMPtr);

  
  while(1){
    
  // waiting for Startbutton to be pressed
  SMPtr = (SM*)shmat(shID, 0, 0);  
  SMPtr->StartButtonPressed = 0;
  shmdt(SMPtr);
  do{
    SMPtr = (SM*)shmat(shID, 0, 0);    
    localSM.StartButtonPressed = SMPtr->StartButtonPressed;
    shmdt(SMPtr);
    usleep(1000);
  }while( !(localSM.StartButtonPressed) );
  
  
  for(char i = 3; i <= 10; i--){    
    SMPtr = (SM*)shmat(shID, 0, 0);
    SMPtr->LED_Number = i;
    shmdt(SMPtr);
    sleep(1);    
  }  

  SMPtr = (SM*)shmat(shID, 0, 0);
  SMPtr->LED_Number = 255;
  shmdt(SMPtr);


  string name = getTerminalOutput("(date +\"%d.%m.%y,%k:%m:%S\")");
  name.erase(name.end() - 1, name.end());
  cout << "Name von Bild: " << name << endl;
  // shoot photos 
  system( ( shoot[0] + name + shoot[1] ) .c_str());

  sleep(1);
  SMPtr = (SM*)shmat(shID, 0, 0);
  SMPtr->LED_Spin = 1;
  shmdt(SMPtr);

  sleep(1);
  // wait for all photos to be taken
  while( getTerminalOutput("pidof gphoto2") != "" ){
    usleep(10000); // 10ms
  };
  
  SMPtr = (SM*)shmat(shID, 0, 0);
  SMPtr->LED_Spin = 0;
  shmdt(SMPtr);
  
  cout << "Starte Bearbeitung" << endl;
  system( ("sudo ./after " + name + " &").c_str() );
  // start New Thread:
  //  - edit picture
  //  - display
  //  - wait if Upload button is pressed
  //  - yes: "Upload"

  }

}



string getTerminalOutput( string cmd ){
  string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2<&1");

  stream = popen(cmd.c_str(), "r");
  if( stream ){
    while(!feof(stream))
      if(fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
  }
  return data;
}

