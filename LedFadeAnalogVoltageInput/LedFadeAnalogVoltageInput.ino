/*
  Button
 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int analogin = A0;     // the number of the analog input pin
const int ledPin =  13;      // the number of the LED pin
const int fadingLed = 9;

// variables will change:
int interval = 0;
int ledState = 0;
unsigned long previousMillis = 0;
int analogVal = 0;
float voltage = 0;
int brightness = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the analog pin as an input:
  pinMode(analogin, INPUT);     
  Serial.begin(9600);
}

void loop(){
  // read the state of the pushbutton value:
  analogVal = analogRead(analogin);
  interval = ( 1024 - analogVal );
  voltage = analogVal * 0.0048828125;
  brightness = analogVal * 0.2490234375;
  Serial.println( voltage );
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    analogWrite(fadingLed, brightness);
  }
}
