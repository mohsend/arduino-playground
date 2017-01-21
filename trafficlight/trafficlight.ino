#include <SevenSegment.h>

int s_clock = 7; // pin number to clock
int s_data = 8; // pin number to data
sevenSegment seg(s_clock, s_data); 

void setup() {
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
}

void loop() {
  // turn on red light
  digitalWrite(0, HIGH);
  countDown(10);
  digitalWrite(0, LOW);
  
  // turn on green light
  digitalWrite(2, HIGH);
  countDown(8);
  digitalWrite(2, LOW);
  
  // turn on yellow light
  digitalWrite(1, HIGH);
  delay(1500);
  digitalWrite(1, LOW);
}

void countDown(int secs) {
   // show numbers from -5 to 34
  // out of range numbers desplay error sign
  for (int i = secs; i >= 0 ; i--) {
    seg.showHex(i); // display number in Hexadecimal
    delay(1000);
  }
}

