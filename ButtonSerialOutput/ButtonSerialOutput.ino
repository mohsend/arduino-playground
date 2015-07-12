#include <Button.h>

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  Button b2(2); 
  Serial.begin(9600);
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    if (lastState == 1) {
      Serial.println(pushes++);
    }
    lastState = 0;
  } else {
    lastState = 1;
  }
  
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > (pushes * 10) ) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}
