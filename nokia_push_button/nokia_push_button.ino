#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Software SPI (slower updates, more flexible pin options):
// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 7 - LCD chip select (CS)
// pin 6 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);

boolean buttonState = 0;
boolean lastState = 0;
int pushes = 0;

boolean debounce(int pin);

void setup()   {
  pinMode(8, INPUT);
  Serial.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  display.display(); // show splashscreen
  delay(1000);
}

void loop() {
    if (debounce(8))
    {
      printnum(++pushes);
      Serial.println(pushes, DEC);
    }
}

void printnum(int x) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(10,10);
  
  display.println(x);
  display.display();
}

boolean debounce(int pin)
{
  static boolean lastState = LOW;
  buttonState = digitalRead(pin);
  if (buttonState == LOW)
  {
    if (lastState == HIGH)
    {
      lastState = LOW;
      delay(22);
      return true;
    }
    lastState = LOW;
  }
  else
  {
    lastState = HIGH;
  }
  
  return false;
}
