<<<<<<< HEAD
// compilieren mit g++ main.c -o main -lcurl

// http://widgets.vvo-online.de/abfahrtsmonitor/Abfahrten.do?hst=südhöhe
#include <iostream>
#include <vector>
#include <curl/curl.h> // apt-get install libcurl4-gnutls-dev
#include <string>
#include <unistd.h>
#include "ST7565.h"
#define contrast 0x20

using namespace std;

typedef vector<string> line;

vector<line> parseHTML( string html );


vector<line> haltestelle;


// TODO: get Raspberry GPIO Running http://raspberrypiguide.de/howtos/raspberry-pi-gpio-how-to/
// http://www.element14.com/community/servlet/JiveServlet/previewBody/73950-102-4-309126/GPIO_Pi2.png?01AD=3GZgWMTpg1jDSMy2YO8hD1OqtjdCp9XqniBSBHoUhsn1I8J5raM6jOQ&01RI=B51CF70A37220D9&01NA=na

 
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char** argv){
  cout << "Hallo Welt" << endl;
  CURL *curl;
  CURLcode res;
  string body;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://widgets.vvo-online.de/abfahrtsmonitor/Abfahrten.do?hst=südhöhe");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    haltestelle = parseHTML( body ); 
    for(unsigned i = 0; i < haltestelle.size(); i++){
       for(unsigned j = 0; j < haltestelle[i].size(); j++)
           cout << haltestelle[i][j] << " ";
       cout << endl;
    }
  }
  

  // initialize screen
  //st7565_init(contrast);

  // clear buffer and screen
  //st7565_clear();

  // Hello World
  //char helloworld[] = "Hello World";
  //drawstring(1, 1, helloworld);

  // update screen
  //st7565_display();

  // main-loop
  //while(1){
    // get bus/tram information

    
    // parse information into vector<line>
    
    // prepare information 
  //}
 
}

/* --------------------------------
    parsing functions
   --------------------------------
*/ 


string parseInfo( string html ){
   return html.substr(1, html.size()-2);
}

line parseLine( string html ){   
   line help;
   for(int i = 0; i < html.size(); i++){
      if( html.at(i) == ',') 
      {
         i++;         
         continue;
      }
      help.push_back( parseInfo( html.substr (i, html.find(',',i)-i ) ) );           
      i = html.find(',',i);
      if( i < 0 ) break;
   }
   return help;
}

vector<line> parseHTML( string html ){
   vector<line> halt;
   for(int i = 0; i < html.size(); i++){
      if( html.at(i) == '[' || html.at(i) == ',' || html.at(i) == ']') 
      {
         i++;
         continue;
      }
      halt.push_back( parseLine(html.substr (i, html.find(']',i)-i ) ) );      
      i = html.find(']',i);
      if( i < 0 ) break;
   }
   return halt;
}
=======
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

>>>>>>> add2121e221b1cc10d810209c22d07195b7cad08
