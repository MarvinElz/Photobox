// compilieren mit g++ -std=c++0x led.c -o led -lwiringPi
#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <string>
#include "led.h"

using namespace std;

int counter = 1;

char table[] = { 
		  0x3f,  // 00111111  (0)
		  0x06,  // 00000110  (1)
 		  0x5b,  // 01011011  (2)
		  0x4f,  // 01001111  (3)
		  0x66,  // 01100110  (4)
		  0x6d,  // 01101101  (5)
		  0x7d,  // 01111101  (6)
		  0x07,  // 00000111  (7)
		  0x7f,  // 01111111  (8)
		  0x6f   // 01101111  (9)
 		};

void clear(){
  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
  digitalWrite(DP, LOW);
}

void spin(char times){
  clear();
  while( times-- ){
  digitalWrite(segA, 1);
  usleep(200000);
  digitalWrite(segA, 0);
  digitalWrite(segB, 1);
  usleep(200000);
  digitalWrite(segB, 0);
  digitalWrite(segC, 1);
  usleep(200000);
  digitalWrite(segC, 0);
  digitalWrite(segD, 1);
  usleep(200000);
  digitalWrite(segD, 0);
  digitalWrite(segE, 1);
  usleep(200000);
  digitalWrite(segE, 0);
  digitalWrite(segF, 1);
  usleep(200000);
  digitalWrite(segF, 0);
  }
}

void write( char value ){
  if(value > 9){
    clear();
    return;
  }  
  digitalWrite(segA, table[value] & (1<<0) );
  digitalWrite(segB, table[value] & (1<<1) );
  digitalWrite(segC, table[value] & (1<<2) );
  digitalWrite(segD, table[value] & (1<<3) );
  digitalWrite(segE, table[value] & (1<<4) );
  digitalWrite(segF, table[value] & (1<<5) );
  digitalWrite(segG, table[value] & (1<<6) );
  digitalWrite(DP,   table[value] & (1<<7) );    
}

int main(int argc, char** argv){
  cout << "Hallo Welt" << endl;

  
  wiringPiSetup();

    // set pin directions
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(DP  , OUTPUT);    

  
  char i = 10;
  while(i--){
    write(i);
    sleep(1);
  }
  
  spin(10);

  clear();
   

}
