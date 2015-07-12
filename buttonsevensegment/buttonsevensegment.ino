#include <SevenSegment.h>

const int _button = 2;

int buttonState = 0;
int lastState = 0;
int pushes = 0;

sevenSegment disp(6,7);

// the setup routine runs once when you press reset:
void setup() {
  for (int i = 8; i <= 11; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(_button, INPUT);
  
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  
  buttonState = digitalRead(_button);
  if (buttonState == LOW) {
    if (lastState == 1) {
      Serial.println(pushes++, BIN);
      disp.showHex(pushes);
      delay(15);
    }
    lastState = 0;
  } else {
    lastState = 1;
  }
  
}
