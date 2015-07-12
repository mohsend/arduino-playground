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
  display.clearDisplay();
}

void loop() {
  
  buttonState = digitalRead(8);
  if (buttonState == LOW) {
    if (lastState == 1) {
      printnum(++pushes);
      display.fillCircle(display.width()/2, display.height()/2, pushes, BLACK);
      display.display();
      Serial.println(pushes, DEC);
      delay(22);
    }
    lastState = 0;
  } else {
    lastState = 1;
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
