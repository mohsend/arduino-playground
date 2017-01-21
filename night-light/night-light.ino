/*
 Fade

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 This example code is in the public domain.
 */

int light = 9;           // the pin that the LED is attached to
int button = 8;
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
long lastActivity = 0;
long sleepAfter = 1 * 6 * 1000;
int isOn = 1;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(light, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  if (digitalRead(button)) {
    changeBrightness();
    lastActivity = millis();
  }
  if (isOn) {
    if (millis() - lastActivity > sleepAfter) {
      turnOff();
    }
  }
}

void turnOff() {
  for (int b = brightness; b > 5; b -= 5) {
    analogWrite(light, b);
    Serial.print("Fade Off: ");
    Serial.println(b);
    delay(1000);
  }
  digitalWrite(light, LOW);
  isOn = 0;
}

void changeBrightness() {
   // set the brightness of pin 9:
  analogWrite(light, brightness);
  Serial.print("Button: ");
  Serial.println(brightness);
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  delay(100);
}

