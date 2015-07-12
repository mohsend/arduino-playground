#include <Button.h>

Button b2(2); 

void setup() {
  // initialize the LED pin as an output:
  pinMode(13, OUTPUT);      
  attachInterrupt(0, debounceInterrupt, RISING);
  // initialize the pushbutton pin as an input:
  Serial.begin(9600);
}

void loop() {
}

void debounceInterrupt() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    Interrupt();
    last_micros = micros();
  }
}

void Interrupt() {
  static int pushes = 0;
  Serial.println(pushes++);
}


void loop(){
  // read the state of the pushbutton value:
  if (b2.debounce())
  {
    static int pushes = 0;
    Serial.println(pushes++);
  }
}
