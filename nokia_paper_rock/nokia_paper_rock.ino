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
}

void loop() {
  
  buttonState = digitalRead(8);
  if (buttonState == LOW) {
    if (lastState == 1) {
      printrockpaper(random(1, 4), 1);
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

void printrockpaper(int computer, int human) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  
  display.setCursor(0,0);
  display.println("CPU");
  display.drawLine(48,0,48,20, BLACK);
  display.drawLine(0,20,84,20, BLACK);
  display.setCursor(50,0);
  display.println("USER");
  display.setCursor(0,10);
  switch (computer) {
    case 1:
      display.println("rock");
      break;
    case 2:
      display.println("paper");
      break;
    case 3:
      display.println("scissors");
      break;
  }
  
  display.setCursor(0,30);
  display.println("YOU WIN!");
  display.display();
}
