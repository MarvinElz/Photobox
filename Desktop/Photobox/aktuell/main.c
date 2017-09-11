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

// commands for taking the photos
string shoot[] = {"sudo gphoto2 --capture-image-and-download --keep --interval 4 --frames 4 --force-overwrite --filename \"raw/", 
		  "_%01n.jpg\" &"};

int main(int argc, char** argv){
  cout << "Photobox Version 1.0" << endl;

//  initialization  *************************
  // create shared memory
  shID = shmget(2404, MAXSM, IPC_CREAT | 0666);
  SMPtr = (SM*)shmat(shID, 0, 0);
  memcpy(SMPtr, &localSM, MAXSM);
  shmdt(SMPtr);

  // Starte led (vorher vorherige Instanzen von led löschen)
  system( ("sudo kill " +  getTerminalOutput("pidof led") ).c_str() );
  system("sudo ./led &");

  // Starte button (vorher vorherige Instanzen von button löschen)
  system( ("sudo kill " + getTerminalOutput("pidof button") ).c_str() );
  system("sudo ./button &");
  cout << "Initialisierung fertig" << endl;
  
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
  
  // count-down
  for(char i = 3; i <= 10; i--){    
    SMPtr = (SM*)shmat(shID, 0, 0);
    SMPtr->LED_Number = i;
    shmdt(SMPtr);
    sleep(1);    
  }  

  // Led off
  SMPtr = (SM*)shmat(shID, 0, 0);
  // only numbers between 0 - 9 can be display, everything else clears the display
  SMPtr->LED_Number = 255;
  shmdt(SMPtr);

  // get current Date + Time -> name of picture
  // e.g. 29.06.2016 11:36:31  ->  2016_06_29_11_36_31 
  string name = getTerminalOutput("(date +\"20%y_%m_%d_%H_%M_%S\")");
  name.erase(name.end() - 1, name.end());
  cout << "Name von Bild: " << name << endl;

  // shoot photos 
  system( ( shoot[0] + name + shoot[1] ) .c_str());

  // Delay (experimentell bestimmt)
  sleep(1);

  // Led-Spin
  SMPtr = (SM*)shmat(shID, 0, 0);
  SMPtr->LED_Spin = 1;
  shmdt(SMPtr);

  sleep(1);
  
  // wait for all photos to be taken
  while( getTerminalOutput("pidof gphoto2") != "" ){
    usleep(10000); // 10ms
  };
    
  // Led-Spin off
  SMPtr = (SM*)shmat(shID, 0, 0);
  SMPtr->LED_Spin = 0;
  shmdt(SMPtr);
  
  
  cout << "Starte Bearbeitung" << endl;
  system( ("sudo ./after " + name + " &").c_str() );

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

