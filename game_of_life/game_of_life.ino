#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "GoL.hpp"

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
gameOfLife gol;

void setup()
{
  Serial.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

  // draw a single pixel
  
}

void loop()
{
  gol.nextgen();
  show();
}

void show()
{
  for (int j = 0; j < GOL_WIDTH; j++)
  {
    for (int i = 0; i < GOL_HEIGHT; i++)
    {
      display.drawPixel(i, j, gol.getLife(i, j));
    }
  }
  display.display();
  delay(800);
  display.clearDisplay();
}
