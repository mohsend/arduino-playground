 /* 
  Debounce
  
  Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
  press), the output pin is toggled from LOW to HIGH or HIGH to LOW.  There's
  a minimum delay between toggles to debounce the circuit (i.e. to ignore
  noise).  
  
  created 28 October 2009
  by Riaan Cornelius
  
  
  */

 // constants won't change. They're used here to 
 // set pin numbers:
 const int buttonPin = 0;     // the number of the pushbutton pin
 const int ledPin =  13;      // the number of the LED pin for testing
 
 const int BUTTON1 = 1;
 const int BUTTON2 = 2;
 const int BUTTON3 = 3;
 const int BUTTON4 = 4;
 const int BUTTON5 = 5;
 
 const int BUTTON1LOW = 1010;
 const int BUTTON1HIGH = 1016;
 const int BUTTON2LOW = 990;
 const int BUTTON2HIGH = 1000;
 const int BUTTON3LOW = 970;
 const int BUTTON3HIGH = 989;
 const int BUTTON4LOW = 920;
 const int BUTTON4HIGH = 940;
 const int BUTTON5LOW = 775;
 const int BUTTON5HIGH = 800;
 
 // Variables will change:
 int ledState = HIGH;         // the current state of the output pin
 int buttonState;             // the current reading from the input pin
 int lastButtonState = LOW;   // the previous reading from the input pin

 // the following variables are long's because the time, measured in miliseconds,
 // will quickly become a bigger number than can be stored in an int.
 long lastDebounceTime = 0;  // the last time the output pin was toggled
 long debounceDelay = 50;    // the debounce time; increase if the output flickers

 void setup() {
   pinMode(buttonPin, INPUT);
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600);
 }

 void loop() {
   // read the state of the switch into a local variable:
   int reading = analogRead(buttonPin);   
   int tmpButtonState = LOW;             // the current reading from the input pin
   
   if(reading>BUTTON5LOW && reading<BUTTON5HIGH){
     //Read switch 5
     tmpButtonState = BUTTON5;
   }else if(reading>BUTTON4LOW && reading<BUTTON4HIGH){
     //Read switch 4
     tmpButtonState = BUTTON4;
   }else if(reading>BUTTON3LOW && reading<BUTTON3HIGH){
     //Read switch 3
     tmpButtonState = BUTTON3;
   }else if(reading>BUTTON2LOW && reading<BUTTON2HIGH){
     //Read switch 2
     tmpButtonState = BUTTON2;
   }else if(reading>BUTTON1LOW && reading<BUTTON1HIGH){
     //Read switch 1
     tmpButtonState = BUTTON1;
   }else{
     //No button is pressed;
     tmpButtonState = LOW;
   }

   // check to see if you just pressed the button 
   // (i.e. the input went from LOW to a buttonState),  and you've waited 
   // long enough since the last press to ignore any noise:  

   // If the switch changed, due to noise or pressing:
   if (tmpButtonState != lastButtonState) {
     // reset the debouncing timer
     lastDebounceTime = millis();
   } 

   if ((millis() - lastDebounceTime) > debounceDelay) {
     // whatever the reading is at, it's been there for longer
     // than the debounce delay, so take it as the actual current state:
     buttonState = tmpButtonState;
     if ((buttonState != 0)) {
      Serial.println(buttonState);
     }
   }

   // save the reading.  Next time through the loop,
   // it'll be the lastButtonState:
   lastButtonState = tmpButtonState;
   
   // set the LED using the state of the button for testing:
   switch(buttonState){
     case BUTTON1:
     digitalWrite(ledPin, buttonState>0);
     break;
     case BUTTON2:
     digitalWrite(ledPin, buttonState>0);
     break;
     case BUTTON3:
     digitalWrite(ledPin, buttonState>0);
     break;
     case BUTTON4:
     digitalWrite(ledPin, buttonState>0);
     break;
     case BUTTON5:
     digitalWrite(ledPin, buttonState>0);
     break;
   }
 }


