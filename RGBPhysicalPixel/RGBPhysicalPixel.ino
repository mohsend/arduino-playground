/*
  Physical Pixel
 
 An example of using the Arduino board to receive data from the 
 computer.  In this case, the Arduino boards turns on an LED when
 it receives the character 'H', and turns off the LED when it
 receives the character 'L'.
 
 The data can be sent from the Arduino serial monitor, or another
 program like Processing (see code below), Flash (via a serial-net
 proxy), PD, or Max/MSP.
 
 The circuit:
 * LED connected from digital pin 13 to ground
 
 created 2006
 by David A. Mellis
 modified 30 Aug 2011
 by Tom Igoe and Scott Fitzgerald
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/PhysicalPixel
 */

#define RED      10
#define GREEN    9
#define BLUE     8


int incomingByte;      // a variable to read incoming serial data into
void setColor (int, int, int);

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  // see if there's incoming serial data:
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    switch (incomingByte) {
     case 'r':
       setColor(1,0,0);
       break;
     case 'g':
       setColor(0,1,0);
       break;
     case 'b':
       setColor(0,0,1);
       break;
     case 'o':
       setColor(0,0,0);
       break;
    }
  }
}

void setColor (int r, int g, int b) {
  
  if ( r == 1 ) {
    digitalWrite(red, HIGH);
  } else {
    digitalWrite(red, LOW);
  }

  if ( g == 1 ) {
    digitalWrite(green, HIGH);
  } else {
    digitalWrite(green, LOW);
  }

  if ( b == 1 ) {
    digitalWrite(blue, HIGH);
  } else {
    digitalWrite(blue, LOW);
  }
}
