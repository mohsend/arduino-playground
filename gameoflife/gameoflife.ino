/*
convey's game of life
*/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


#define	HEIGHT    42
#define WIDTH    32

bool life[WIDTH][HEIGHT];

void show(bool life[WIDTH][HEIGHT]);
void nextgen(bool life[WIDTH][HEIGHT]);
void init(bool life[WIDTH][HEIGHT]);

// Software SPI (slower updates, more flexible pin options):
// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 7 - LCD chip select (CS)
// pin 6 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);

void setup()   {
  Serial.begin(9600);
  init(life);
  display.begin();
  display.setContrast(50);

  display.display(); // show splashscreen
  delay(1000);
}

void loop() {
  show(life);
  display.display();
  delay(1000);
  nextgen(life);
}

void show(bool life[WIDTH][HEIGHT])
{
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
		{
                    if (life[i][j])
                      display.drawPixel(j, i, BLACK);
                    else
                      display.drawPixel(j, i, WHITE);
		}
}

void nextgen(bool life[WIDTH][HEIGHT])
{
	// analyze each pixel
	int neighbors[WIDTH][HEIGHT];
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
		{
			// calculate the number of neighbors of the pixel
			neighbors[i][j] = 
				life[i - 1][j - 1] +
				life[i - 1][j] +
				life[i - 1][j + 1] +
				life[i][j - 1] +
				life[i][j] +
				life[i][j + 1] +
				life[i + 1][j - 1] +
				life[i + 1][j] +
				life[i + 1][j + 1];
		}
	// calculate the state of each pixel in the next generation
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
		{
			//1. Any live cell with fewer than two live neighbors dies, as if caused by under-population.
			if (neighbors[i][j] < 2)
				life[i][j] = false;
			//2. Any live cell with two or three live neighbors lives on to the next generation.
				// automaticly done
			//3. Any live cell with more than three live neighbors dies, as if by overcrowding.
			if (neighbors[i][j] > 3)
				life[i][j] = false;
			//4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
			if (neighbors[i][j] == 3)
				life[i][j] = true;
		}
}

void init(bool life[WIDTH][HEIGHT])
{
	// loop through every pixel and assign a random value to it
	for (int i = 0; i < WIDTH; i++)
		for (int j = 0; j < HEIGHT; j++)
			life[i][j] = random(0,2);
}

