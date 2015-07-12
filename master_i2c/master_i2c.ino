#include <Wire.h>

#define LED_PIN 13
byte x = 0;

void setup()
{
  Wire.begin(); // Start I2C Bus as Master
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

}
void loop()
{
  sendstring("hello world!\n");
  delay(450);
}

void sendstring(char *str) {
  int i = 0;
  while (*(str + i)) {
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(*(str + i));              // sends x 
    Wire.endTransmission();    // stop transmitting
    i++;
  }
} 
