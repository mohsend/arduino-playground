/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  delay(10000);
  dialPhone("09192056948");
}

// the loop function runs over and over again forever
void loop() {
  
}

void dialPulse(int n)
{
  if (n == 0)
    n = 10;
  for (int i = 1; i <= n; i++)
  {
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(60);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(60);              // wait for a second
  }
  delay(600);
}

void dialPhone(char* num)
{
  for(int i = 0; num[i]; i++)
  {
    int digit = num[i] - '0';
    Serial.println(digit);
    dialPulse(digit);
  } 
}

