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
// define pins:
#define RED      11
#define GREEN    10
#define BLUE     9
#define BUTTON   2

// functions
void setColor (int, int, int);

// variables
int state = 0;
int buttonState = 0;
int lastState = 0;


void setup() {
  // initialize the pins:
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  
  buttonState = digitalRead(BUTTON);
  if (buttonState == HIGH) {
    lastState = 1;
    delay(10);
  } else {
    if (lastState == 1) {
      state++;
    }
    lastState = 0;
  }
  
  
  if (state == 0) {
    setColor(1,0,0);
  } else if (state == 1) {
    setColor(0,1,0);
  } else if (state == 2) {
    setColor(0,0,1);
  } else {
    setColor(0,0,0);
    state = 0;
  }
}

void setColor (int r, int g, int b) {
  if ( r == 1 ) {
    digitalWrite(RED, HIGH);
  } else {
    digitalWrite(RED, LOW);
  }

  if ( g == 1 ) {
    digitalWrite(GREEN, HIGH);
  } else {
    digitalWrite(GREEN, LOW);
  }

  if ( b == 1 ) {
    digitalWrite(BLUE, HIGH);
  } else {
    digitalWrite(BLUE, LOW);
  }
}
