// compilieren mit g++ -std=c++0x main.c -o main -lwiringPi
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <string>

using namespace std;

int counter = 1;


int main(int argc, char** argv){
  cout << "Hallo Welt" << endl;

  while(counter < 15){  
     std::string s1 = "./shoot " + to_string(counter);
     system(s1.c_str());
     counter++;
  }

}
