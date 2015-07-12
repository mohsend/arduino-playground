const int _clock = 7;
const int _data = 8;


int analog = 0;
  
  
void shiftRegister(int p[8]);
void sevenSegmentHex(int x);

// the setup routine runs once when you press reset:
void setup() {
  pinMode(_clock, OUTPUT);
  pinMode(_data, OUTPUT);
  
  sevenSegmentHex(-1);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  analog = analogRead(A0);
  //sevenSegmentHex(analog / 32); // 0v - 5v => 0 - 1024 => 0 - 31 (0x1F)
  sevenSegmentHex(analog / 51); // 0v - 5v => 0 - 1024 => 0 - 20 (0x14)
  Serial.print("VOLTAGE = " ); 
  Serial.print( (float)(analog * 0.0048828125) );
  Serial.print("   seven segment = 0x" ); 
  Serial.println(analog / 51, HEX);
  delay(500);
}

void shiftRegister(int p[8])
{
    for (int i = 0; i < 8; i++) {
      digitalWrite(_data, p[i]);
      digitalWrite(_clock, HIGH);
      digitalWrite(_clock, LOW);
  }
}

void sevenSegmentHex(int x)
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

int readButtonPushes (int buttonPin) {
  int buttonState = 0;
  int lastState = 0;
  int pushes = 0;
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    if (lastState == 1) {
      Serial.println(pushes++);
    }
    lastState = 0;
  } else {
    lastState = 1;
  }
}
