#include <SevenSegment.h>

int incomingInt = 0;  
sevenSegment ss(7,8);

// the setup routine runs once when you press reset:
void setup() {
  ss.showHex(-1);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
   if (Serial.available() > 0) {
    incomingInt = Serial.parseInt();
    ss.showHex(incomingInt);
  }
  
}
