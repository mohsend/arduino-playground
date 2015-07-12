const int _clock = 9;
const int _data = 8;

void shiftRegister(int p);
void sevenSegmentHex(int x);

// the setup routine runs once when you press reset:
void setup() {
  
}

// the loop routine runs over and over again forever:
void loop() {
  for (int i = -5; i < 35; i++) {
    sevenSegmentHex (i);
    delay(500);
  }
}

void shiftRegister(int p)
{
    for (int i = 7; i > -1; i--) {
      boolean state = bitRead(p, i);
      digitalWrite(_data, state);
      digitalWrite(_clock, HIGH);
      digitalWrite(_clock, LOW);
  }
}

void sevenSegmentHex(int x)
{
  int digit[] = {
    0x77, //0
    0x41, //1
    0x3B, //2
    0x6B, //3
    0x4D, //4
    0x6E, //5
    0x7E, //6 
    0x43, //7
    0x7F, //8
    0x6F, //9
    0x5F, //a
    0x7C, //b,
    0x38, //c
    0x79, //d
    0x3E, //e
    0x1E, //f
    0x80, // .
    0x00, // [ ]
    0x2A  // =
  };
  
  
  if ( x == -1) {
    // [ ]
    shiftRegister(digit[17]);
  } else if ( x == -2 ) {
    // .
    shiftRegister(digit[16]);
  } else if ( (x > -1) && (x < 16) ) {
    shiftRegister(digit[x]);
  } else if ( (x > 15) && (x < 32) ) {
    shiftRegister(digit[(x - 16)] + 128);
  } else {
    shiftRegister(digit[18]);
  }
}
