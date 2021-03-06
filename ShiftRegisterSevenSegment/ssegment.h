/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"

SSegment::SSegment(int clock, int data)
{
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  int _clock = clock;
  int _data = data;
}

void SSegment::shiftRegister(int p[8])
{
    for (int i = 0; i < 8; i++) {
      digitalWrite(_data, p[i]);
      digitalWrite(_clock, HIGH);
      digitalWrite(_clock, LOW);
  }
}

void SSegment::sevenSegmentHex(int x)
{
  int digit[19][8] = {
    {0,1,1,1,0,1,1,1}, //0
    {0,1,0,0,0,0,0,1}, //1
    {0,0,1,1,1,0,1,1}, //2
    {0,1,1,0,1,0,1,1}, //3
    {0,1,0,0,1,1,0,1}, //4
    {0,1,1,0,1,1,1,0}, //5
    {0,1,1,1,1,1,1,0}, //6 
    {0,1,0,0,0,0,1,1}, //7
    {0,1,1,1,1,1,1,1}, //8
    {0,1,1,0,1,1,1,1}, //9
    {0,1,0,1,1,1,1,1}, //a
    {0,1,1,1,1,1,0,0}, //b
    {0,0,1,1,1,0,0,0}, //c
    {0,1,1,1,1,0,0,1}, //d
    {0,0,1,1,1,1,1,0}, //e
    {0,0,0,1,1,1,1,0}, //f
    {1,0,0,0,0,0,0,0}, // .
    {0,0,0,0,0,0,0,0}, // [ ]
    {0,0,1,0,1,0,1,0}  // =
  };
  
  int big[8] = {1,0,0,0,0,0,0,0};
  
  if ( x == -1) { // [ ]
    shiftRegister(digit[17]);
  } else if ( x == -2 ) { // .
    shiftRegister(digit[16]);
  } else if ( (x > -1) && (x < 16) ) {
    shiftRegister(digit[x]);
  } else if ( (x > 15) && (x < 32) ) {
    for (int i = 1; i < 8; i++) {
      big[i] = digit[(x - 16)][i];
    }
    shiftRegister(big);
  } else {
    shiftRegister(digit[18]);
  }
}
