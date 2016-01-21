// compilieren mit g++ -std=c++0x after.c -o after -lwiringPi
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <string>
#include <string.h>
#include "sharedMemory.h"
#include <sys/time.h>
#include <unistd.h>

using namespace std;

struct timeval start;
struct timeval mid;


int shID;
SM *SMPtr;
SM localSM;

string edit[] = {"sudo java -jar PhotoCombine.jar ",
		 " /home/pi/Desktop/Photobox/aktuell/raw/ /home/pi/Desktop/Photobox/aktuell/edit/" };
string print[] = {"lp -d HP_Officejet_6500_E710a-f -o position=bottom /home/pi/Desktop/Photobox/aktuell/edit/",
		  ".jpg &"};

// arg: name of image
int main(int argc, char** argv){
  //edit pictures
  system( ( edit[0] + (string)argv[1] + edit[1] ) .c_str());
  
  // Display created photo
  cout << "Display image" << endl;
  // system(feh ...
   
  cout << "Print: " << ( print[0] + (string)argv[1] + print[1] ) .c_str() << endl;
  system( ( print[0] + (string)argv[1] + print[1] ) .c_str());

  // allocate memory
  shID = shmget(2404, MAXSM, 0666);

  SMPtr = (SM*)shmat(shID, 0, 0);
  SMPtr->UploadButtonPressed = 0;
  localSM.UploadButtonPressed = 0;
  shmdt(SMPtr);

  gettimeofday(&start, NULL);
    

  do{      
    if( !localSM.UploadButtonPressed )
    {   
       SMPtr = (SM*)shmat(shID, 0, 0);
       if( SMPtr->UploadButtonPressed ){   
         localSM.UploadButtonPressed = 1;
	 cout << "Upload" << endl;      
       }
       shmdt(SMPtr);   
    }
    usleep(1000);
    gettimeofday(&mid, NULL); 	
  }while( mid.tv_sec - start.tv_sec < 60 );
  
  // Display deaktivieren
  cout << "Display off" << endl;

  return 0;
}
