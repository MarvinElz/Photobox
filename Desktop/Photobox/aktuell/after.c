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
#include <fstream>


using namespace std;

struct timeval start;
struct timeval mid;


int shID;
SM *SMPtr;
SM localSM;

// Commands for editing picutres
string edit[] = {"sudo java -jar PhotoCombine.jar ",
		 " /home/pi/Desktop/Photobox/aktuell/raw/ /home/pi/Desktop/Photobox/aktuell/edit/" };

string show[] = {"sudo ./show /home/pi/Desktop/Photobox/aktuell/edit/",
		 ".jpg"};
		 		 
// Commands for printing				  
string print[] = {"lp -d HP_Officejet_6500_E710a-f -o fit-to-page -o position=bottom /home/pi/Desktop/Photobox/aktuell/edit/",
		  ".jpg &"};

// arg: name of image
int main(int argc, char** argv){
  //edit pictures
  system( ( edit[0] + (string)argv[1] + edit[1] ) .c_str());   

  cout << "Print: " << ( print[0] + (string)argv[1] + print[1] ) .c_str() << endl;
  
  // print picture
  system( ( print[0] + (string)argv[1] + print[1] ) .c_str());

  system( (show[0] + (string)argv[1] + show[1] ) .c_str() );

  return 0;
}

/*  code for saving pictures that should be uploaded later

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
	 // write to file
	 ofstream out("uploadList.txt", ios::out |ios::app);  
  	 if( out )
	 {
	   out << argv[1] << endl;
	 }
	 out.close();
   
       }
       shmdt(SMPtr);   
    }
    usleep(1000);
    gettimeofday(&mid, NULL); 	
  }while( mid.tv_sec - start.tv_sec < 60 );
  
  // Display deaktivieren
  cout << "Display off" << endl;
*/