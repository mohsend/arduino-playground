#include <SevenSegment.h>

const int _button = 2;

int buttonState = 0;
int lastState = 0;
int pushes = 0;

void binledout(int p);
sevenSegment disp(6,7);

// the setup routine runs once when you press reset:
void setup() {
  for (int i = 8; i <= 11; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(_button, INPUT);
  
  Serial.begin(9600);
  disp.showHex(0);
}

// the loop routine runs over and over again forever:
void loop() {
  
  buttonState = digitalRead(_button);
  if (buttonState == LOW) {
    if (lastState == 1) {
      Serial.println(pushes++, BIN);
      binledout(pushes);
      disp.showHex(pushes);
      delay(15);
    }
    lastState = 0;
  } else {
    lastState = 1;
  }
  
}

void binledout(int p)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(i + 8 ,bitRead(p, i));
  }
}
