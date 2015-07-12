#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Software SPI (slower updates, more flexible pin options):
// pin 9 - Serial clock out (SCLK)
// pin 10 - Serial data out (DIN)
// pin 11 - Data/Command select (D/C)
// pin 12 - LCD chip select (CS)
// pin 13 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 11, 12, 13);

void drawAngle (int center_x, int center_y, float angle);

void setup()   {
  Serial.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(60);

  display.display(); // show splashscreen
  delay(1000);
}

void loop() {
  delay(1000);
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0,0);
  
  long int seconds = millis() / 1000;
  display.print(seconds / 60);
  display.print(":");
  display.println(seconds % 60);
  
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0,41);
  display.print("since start! ");
  display.write(2);
  drawAngle(70, 25, seconds * (0.105), 10);
  display.drawCircle(70, 25, 10, BLACK);
  display.display();
}

void drawAngle (int center_x, int center_y, float angle, int length) {
  angle -= 1.5708;
  display.drawLine(center_x, center_y, (center_x + cos(angle) * length), (center_y + sin(angle) * length), BLACK);
}
