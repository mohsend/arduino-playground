#include <SevenSegment.h>

int incomingInt = 0;

int two_power(int);

// the setup routine runs once when you press reset:
void setup() {
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  
  lightall();
  
  Serial.begin(9600);
  delay(2000);
}

// the loop routine runs over and over again forever:
void loop() {
//  for ( int i = A1; i < A6; i++) {
//    for (int j = 2; j < 7; j++) {
//      light(i,j);
//      delay(1);
//    }
//  }


//for (int i = 0; i < 0x20; i++) {
//    line (1, i);
//    Serial.println(i, HEX);
//    delay(500);
//  }

  //plus();
  //cross();
  smiley();
}

void light (int x, int y) {
  off();
  digitalWrite(x, LOW);
  digitalWrite(y, HIGH);
}

void off () {  
  for ( int i = A1; i < A6; i++) {
    for (int j = 2; j < 7; j++) {
      digitalWrite(j, LOW);
    }
    digitalWrite(i, HIGH);
  }
}

void pixel (int x, int y) {
//  light(x + A0, y + 1);
//  delay(1);
  line(x, two_power(y) );
}

void cross() {
//  for ( int i = 1; i < 6; i++) {
//    pixel(i,i);
//    pixel(i,6-i);
//  }

  line(1, 0x11);
  line(2, 0x0A);
  line(3, 0x04);
  line(4, 0x0A);
  line(5, 0x11);
}

void plus () {
//  for ( int i = 1; i < 6; i++) {
//    pixel(3,i);
//    pixel(i,3);
//  }
  line(1, 0x4);
  line(2, 0x4);
  line(3, 0x1F);
  line(4, 0x4);
  line(5, 0x4);
}

void lightall() {
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
  
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
}

void line (int line, int pixels) {
  off();
  for (int i = 0; i < 5; i++) {
    boolean state = bitRead(pixels, i);
    digitalWrite( (i + 2), state);
  }
  digitalWrite( (line + A0), LOW);
  delay(1);
}

void smiley() {
  int lines[5] = {0x08,
    0x12,
    0x10,
    0x12,
    0x08};
  display_array(lines);
}

int two_power(int x) {
 // returns 2^x
 int temp = 1;
  for (int i = 0; i < x; i++) {
    temp *= 2;
  }
  return temp;
}

void display_array(int lines[5]) {
  line(1, lines[0]);
  line(2, lines[1]);
  line(3, lines[2]);
  line(4, lines[3]);
  line(5, lines[4]);
}
