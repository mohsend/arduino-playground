/* ConwaysLife --- Conway's Game of Life on Nokia 1202 LCD  2012-09-22 */
/* Copyright (c) 2012 John Honniball                                   */

/* Released under the GNU Public Licence (GPL) */

#include <SPI.h>
#include "font.h"

// Direct port I/O defines for Arduino with ATmega328
// Change these if running on Mega Arduino
#define LCDOUT PORTB
#define CS     7
#define SDA    4
#define SCLK   3

// Connections to Nokia 1202 LCD, via CD4050 level-shifter
#define slaveSelectPin 9
#define SDAPin 11
#define SCLKPin 13

// Size of Nokia 1202 LCD screen
#define MAXX 96
#define MAXY 68
#define MAXROWS 9  // 9 rows of bytes, last row only half used

// Co-ord of centre of screen
#define CENX (MAXX / 2)
#define CENY (MAXY / 2)

// The following two arrays fill up very nearly all of the RAM
// on the Arduino:

// The cells, as a bit-map, 864 bytes
unsigned char Cells[MAXROWS][MAXX];

// The frame buffer, 864 bytes
unsigned char Frame[MAXROWS][MAXX];


void setup (void)
{
  int i;
  
  //Serial.begin (9600);

  lcd1202_begin ();
  
  clrFrame ();
  
  fillRoundRect (CENX - (3 * 12) - 2, CENY - 6, CENX + (3 * 12) + 2, CENY + 16, 7);
  setText (CENX - (3 * 12), CENY, "Conways Game");
  setText (CENX - (3 * 7), CENY + 8, "of Life");
  
  updscreen ();
  
  delay (2000);
  
  drawInitialCells ();
  
  updscreen ();
  
  delay (2000);  
}


void loop (void)
{
  static long int generation = 0;
  char genstr[16];
  long int start, now;
  int elapsed;

  // Record timer in milliseconds at start of frame cycle
  start = millis ();
  
  // Let one generation pass
  newGeneration ();
  
  // That's now the old generation
  oldGeneration ();
  
  // Overlay generation counter in top left-hand corner
  sprintf (genstr, "%ld", ++generation);
  setText (0, 0, genstr);
  
  // Update LCD for this frame
  updscreen ();
  
  // Work out timing for this frame
  now = millis ();
  elapsed = now - start;
  
  //Serial.print (elapsed);
  //Serial.println ("ms.");
  
  // We'll never have time to actually delay, because we
  // take about 100ms to compute each generation
  if (elapsed < 20)
    delay (20 - elapsed);
}


/* drawInitialCells --- seed the playing area with live cells */

void drawInitialCells (void)
{
  int i;
  
  memset (Cells, 0, sizeof (Cells));
  
//#ifdef F_PENTOMINO
  Cells[CENY / 8][CENX - 1] = 0x02;
  Cells[CENY / 8][CENX] = 0x07;
  Cells[CENY / 8][CENX + 1] = 0x01;
//#endif

#ifdef DIEHARD
  Cells[CENY / 8][CENX - 4] = 0x02;
  Cells[CENY / 8][CENX - 3] = 0x06;
  Cells[CENY / 8][CENX + 1] = 0x04;
  Cells[CENY / 8][CENX + 2] = 0x05;
  Cells[CENY / 8][CENX + 3] = 0x04;
#endif

#ifdef INFINITE3
  for (i = -18; i <= -5; i++)
    if (i != -10)
      Cells[CENY / 8][CENX + i] = 0x01;

  Cells[CENY / 8][CENX - 1] = 0x01;
  Cells[CENY / 8][CENX] = 0x01;
  Cells[CENY / 8][CENX + 1] = 0x01;
  
  for (i = 8; i <= 20; i++)
    if ( i != 15)
      Cells[CENY / 8][CENX + i] = 0x01;
#endif
      
  memcpy (Frame, Cells, sizeof (Frame));
}


/* newGeneration --- execute life for one generation of cells */

int newGeneration (void)
{
  int x, y;
  int ncells = 0;
  int nn;
  unsigned int xp1, xm1;  // X plus/minus one
  unsigned int yp1, ym1;  // Y plus/minus one
  unsigned int yrow, yp1row, ym1row;
  unsigned char ymask, yp1mask, ym1mask;

  // New generation is created in the Frame array, so clear it
  clrFrame ();
  
  for (y = 0; y < MAXY; y++) {
    // Precompute array index and bit-mask for this row of cells
    if (y == 0)
      ym1 = MAXY - 1;
    else
      ym1 = y - 1;
    
    if (y == (MAXY - 1))
      yp1 = 0;
    else
      yp1 = y + 1;
    
    ymask = 1 << (y & 7);
    yrow = y / 8;
    
    ym1mask = 1 << (ym1 & 7);
    ym1row = ym1 / 8;
    
    yp1mask = 1 << (yp1 & 7);
    yp1row = yp1 / 8;

    // Run along this row of cells
    for (x = 0; x < MAXX; x++) {
      // Allow for wrap-around at edges (as if on a toroid)
      if (x == 0)
        xm1 = MAXX - 1;
      else
        xm1 = x - 1;
      
      if (x == (MAXX - 1))
        xp1 = 0;
      else
        xp1 = x + 1;
      
      // Count the number of neighbouring cells that are alive  
      nn = cellAt2 (xm1, ym1row, ym1mask);
      nn += cellAt2 (xm1, yrow, ymask);
      nn += cellAt2 (xm1, yp1row, yp1mask);
      
      nn += cellAt2 (x, ym1row, ym1mask);
      nn += cellAt2 (x, yp1row, yp1mask);

      nn += cellAt2 (xp1, ym1row, ym1mask);
      nn += cellAt2 (xp1, yrow, ymask);
      nn += cellAt2 (xp1, yp1row, yp1mask);
   
      // Now follow the rules for the current cell at (x, y)
      if (cellAt2 (x, yrow, ymask)) {
        if (nn == 2 || nn == 3) {
          setCell (x, y);
          ncells++;  // This cell lives on
        }
        // else
          // Dies by isolation or overcrowding     
      }
      else {  // Current cell is not living
        if (nn == 3) {
          setCell (x, y);
          ncells++;  // New cell is born
        }
      }
    }
  }
  
  return (ncells);
}


/* cellAt --- return state of cell at a given location */

int cellAt (int x, int y)
{
  if (x < 0)
    x = MAXX - 1;
  
  if (x >= MAXX)
    x = 0;
    
  if (y < 0)
    y = MAXY - 1;
  
  if (y >= MAXY)
    y = 0;

  return ((Cells[y / 8][x] & (1 << (y & 7)))? 1: 0);
}


/* cellAt2 --- return state of cell at a given location */

inline int cellAt2 (unsigned int x, unsigned int row, unsigned char mask)
{    
  return ((Cells[row][x] & mask)? 1: 0);
}


/* setCell --- set the state of a cell to be alive */

void setCell (unsigned int x, unsigned int y)
{
  Frame[y / 8][x] |= 1 << (y & 7);
}


/* oldGeneration --- the new generation now becomes the old */

void oldGeneration (void)
{
  memcpy (Cells, Frame, sizeof (Cells));
}


/* clrFrame --- clear the entire frame to white */

void clrFrame (void)
{
  memset (Frame, 0, sizeof (Frame));
}


/* setText --- draw text into buffer using predefined font */

void setText (int x, int y, const char *str)
{
  // This function does not, as yet, allow for pixel row positioning of text.
  // The Y co-ordinate is rounded to the nearest row of display RAM bytes.
  // The font (475 bytes) is held in program memory (Flash) to reduce RAM
  // usage. The AVR is a Harvard architecture machine and needs a special
  // instruction to read program memory, which is implemented in C as the
  // 'pgm_read_byte_near' function.
  int row;
  int i;
  int d;
  
  row = y >> 3;
  
  for ( ; *str; str++) {
    d = (*str - ' ') * 5;
    
    for (i = 0; i < 5; i++, d++) {
      Frame[row][x++] = pgm_read_byte_near (font_data + d);
    }
    
    Frame[row][x++] = 0;
  }
}


/* drawRoundRect --- draw a rounded rectangle */

void drawRoundRect (int x0, int y0, int x1, int y1, int r)
{
  setHline (x0 + r, x1 - r, y0);
  setHline (x0 + r, x1 - r, y1);
  setVline (x0, y0 + r, y1 - r);
  setVline (x1, y0 + r, y1 - r);
  
  drawSplitCircle (x0 + r, y0 + r, x1 - r, y1 - r, r, 1, -1);
}


/* fillRoundRect --- fill a rounded rectangle */

void fillRoundRect (int x0, int y0, int x1, int y1, int r)
{
  int y;
  
  drawSplitCircle (x0 + r, y0 + r, x1 - r, y1 - r, r, 1, 0);
  
  setHline (x0 + r, x1 - r, y0);
  setHline (x0 + r, x1 - r, y1);
  setVline (x0, y0 + r, y1 - r);
  setVline (x1, y0 + r, y1 - r);
  
  for (y = y0 + r; y < (y1 - r); y++)
    clrHline (x0 + 1, x1 - 1, y);
}


/* drawSplitCircle --- draw a split circle with edge and fill colours */

void drawSplitCircle (int x0, int y0, int x1, int y1, int r, int ec, int fc)
{
  // Michener's circle algorithm. Originally coded on the IBM PC
  // with EGA card in 1986.
  int x, y;
  int d;

  x = 0;
  y = r;
  d = 3 - (2 * r);

  if (fc >= 0) {
    while (x < y) {
      splitcfill (x0, y0, x1, y1, x, y, fc);
      if (d < 0) {
        d += (4 * x) + 6;
      }
      else {
        d += (4 * (x - y)) + 10;
        y--;
      }
      x++;
    }
    
    if (x == y)
      splitcfill (x0, y0, x1, y1, x, y, fc);
  }
  
  x = 0;
  y = r;
  d = 3 - (2 * r);

  while (x < y) {
    splitcpts8 (x0, y0, x1, y1, x, y, ec);
    if (d < 0) {
      d += (4 * x) + 6;
    }
    else {
      d += (4 * (x - y)) + 10;
      y--;
    }
    x++;
  }
  
  if (x == y)
    splitcpts8 (x0, y0, x1, y1, x, y, ec);
}


/* cfill --- draw horizontal lines to fill a circle */

static void splitcfill (int x0, int y0, int x1, int y1, int x, int y, int fc)
{
  if (fc) {
    setHline (x0 - x, x1 + x, y1 + y);
    setHline (x0 - x, x1 + x, y0 - y);
    setHline (x0 - y, x1 + y, y1 + x);
    setHline (x0 - y, x1 + y, y0 - x);
  }
  else {
    clrHline (x0 - x, x1 + x, y1 + y);
    clrHline (x0 - x, x1 + x, y0 - y);
    clrHline (x0 - y, x1 + y, y1 + x);
    clrHline (x0 - y, x1 + y, y0 - x);
  }
}


/* splitcpts8 --- draw eight pixels to form the edge of a split circle */

static void splitcpts8 (int x0, int y0, int x1, int y1, int x, int y, int ec)
{
  splitcpts4 (x0, y0, x1, y1, x, y, ec);

// if (x != y)
    splitcpts4 (x0, y0, x1, y1, y, x, ec);
}


/* splitcpts4 --- draw four pixels to form the edge of a split circle */

static void splitcpts4 (int x0, int y0, int x1, int y1, int x, int y, int ec)
{
  if (ec) {
    setPixel (x1 + x, y1 + y);

//  if (x != 0)
      setPixel (x0 - x, y1 + y);

//  if (y != 0)  
      setPixel (x1 + x, y0 - y);

//  if ((x != 0) && (y != 0))
      setPixel (x0 - x, y0 - y);
  }
  else {
    clrPixel (x1 + x, y1 + y);

//  if (x != 0)
      clrPixel (x0 - x, y1 + y);

//  if (y != 0)
      clrPixel (x1 + x, y0 - y);

//  if ((x != 0) && (y != 0))
      clrPixel (x0 - x, y0 - y);
  }
}


/* setVline --- draw vertical line */

void setVline (unsigned int x, unsigned int y1, unsigned int y2)
{
  unsigned int y;
  
  for (y = y1; y <= y2; y++)
    setPixel (x, y);
}


/* clrVline --- draw vertical line */

void clrVline (unsigned int x, unsigned int y1, unsigned int y2)
{
  unsigned int y;
  
  for (y = y1; y <= y2; y++)
    clrPixel (x, y);
}

/* setHline --- set pixels in a horizontal line */

void setHline (unsigned int x1, unsigned int x2, unsigned int y)
{
  unsigned int x;
  unsigned int row;
  unsigned char b;
  
  row = y / 8;
  b = 1 << (y  & 7);
  
  for (x = x1; x <= x2; x++)
    Frame[row][x] |= b;
}


/* clrHline --- clear pixels in a horizontal line */

void clrHline (unsigned int x1, unsigned int x2, unsigned int y)
{
  unsigned int x;
  unsigned int row;
  unsigned char b;

  if (y < MAXY) {  
    row = y / 8;
    b = ~(1 << (y  & 7));
    
    for (x = x1; x <= x2; x++)
      Frame[row][x] &= b;
  }
}


/* setRect --- set pixels in a (non-filled) rectangle */

void setRect (int x1, int y1, int x2, int y2)
{
  setHline (x1, x2, y1);
  setVline (x2, y1, y2);
  setHline (x1, x2, y2);
  setVline (x1, y1, y2);
}


/* fillRect --- set pixels in a filled rectangle */

void fillRect (int x1, int y1, int x2, int y2, int ec, int fc)
{
  int y;
  
  for (y = y1; y <= y2; y++)
    if (fc == 0)
      clrHline (x1, x2, y);
    else if (fc == 1)
      setHline (x1, x2, y);
  
  if (ec == 1) {
    setHline (x1, x2, y1);
    setVline (x2, y1, y2);
    setHline (x1, x2, y2);
    setVline (x1, y1, y2);
  }
  else if (ec == 0) {
    clrHline (x1, x2, y1);
    clrVline (x2, y1, y2);
    clrHline (x1, x2, y2);
    clrVline (x1, y1, y2);
  }
}


/* setPixel --- set a single pixel */

void setPixel (unsigned int x, unsigned int y)
{
  if ((x < MAXX) && (y < MAXY))
    Frame[y / 8][x] |= 1 << (y & 7);
  else {
//  Serial.print ("setPixel(");
//  Serial.print (x);
//  Serial.print (",");    
//  Serial.print (y);
//  Serial.println (")");    
  }
}


/* clrPixel --- clear a single pixel */

void clrPixel (unsigned int x, unsigned int y)
{
  if ((x < MAXX) && (y < MAXY))
    Frame[y / 8][x] &= ~(1 << (y & 7));
  else {
//  Serial.print ("clrPixel(");
//  Serial.print (x);
//  Serial.print (",");
//  Serial.print (y);
//  Serial.println (")");    
  }
}


/* updscreen --- update the physical screen from the buffer */

void updscreen (void)
{
  // This function contains an eight-way unrolled loop. In the Arduino
  // IDE, the default GCC optimisation switch is -Os, which optimises
  // for space. No automatic loop unrolling is done by the compiler, so
  // we do it explicitly here to save a few microseconds.
//  long int before, after;
//  unsigned char r, c;
  unsigned char *p;
  int i;
  
//lcdGotoRC (0, 0);
  lcdSpi (0xB0); // Set page address to 'r'
  lcdSpi (0x10);   // Sets DDRAM column addr - upper 3-bit
  lcdSpi (0x00); // lower 4-bit
  
//  before = micros ();
  
  p = &Frame[0][0];
  
  for (i = 0; i < ((MAXROWS * MAXX) / 8); i++) {
    lcdData (*p++);
    lcdData (*p++);
    lcdData (*p++);
    lcdData (*p++);
    lcdData (*p++);
    lcdData (*p++);
    lcdData (*p++);
    lcdData (*p++);
  }

/*
  The slow way...
  for (r = 0; r < MAXROWS; r++) {
    for (c = 0; c < MAXX; c++) {
      lcdData (Frame[r][c]);
    }
  }
*/

//  after = micros ();
  
//  Serial.print (after - before);
//  Serial.println ("us updscreen");
}


/* lcd1202_begin --- initialise the Nokia 1202 LCD */

void lcd1202_begin (void)
{
  // LCD initialisation code from Greeeg of the Dangerous Prototypes forum.
  // The chip on the Nokia 1202 LCD is an ST Microelectronics STE2007.
  // Greeeg also designed the 5x7 pixel font that's used by 'setText'.
  /* Configure I/O pins on Arduino */
  pinMode (slaveSelectPin, OUTPUT);
  pinMode (SDAPin, OUTPUT);
  pinMode (SCLKPin, OUTPUT);
  
  digitalWrite (slaveSelectPin, HIGH);
  digitalWrite (SDAPin, HIGH);
  digitalWrite (SCLKPin, HIGH);

  SPI.begin ();
  // The following line fails on arduino-0021 due to a bug in the SPI library
  // Compile with arduino-0022 or later
  SPI.setClockDivider (SPI_CLOCK_DIV4);
  SPI.setBitOrder (MSBFIRST);
  SPI.setDataMode (SPI_MODE3);
  
  // For the moment, disable SPI and return the pins to normal I/O use
  SPCR &= ~(1<<SPE);
  
  /* Start configuring the STE2007 LCD controller */
// LCDOUT |= RESET; // Hard reset
   
   lcdSpi (0xE2); // SW reset.
   
// lcdSpi (0xA5); // Power saver ON, display all pixels ON
   lcdSpi (0xA4); // Power saver OFF
   lcdSpi (0x2F); // Power control set
// lcdSpi (0xAE); // Display OFF, blank
   lcdSpi (0xAF); // Display ON
  
// These next two commands (A0/A1) don't work on my 1202 LCD
// lcdSpi (0xA0);   // Display not flipped
// lcdSpi (0xA1);   // Display flipped

// 'Normal' makes the bit-mapped display work top/left to
// bottom/right when the LCD is oriented with the connector
// at the top edge. I don't know which way up the LCD is
// supposed to be mounted in the phone, but this setup
// makes more sense to me on the breadboard.
   lcdSpi (0xC0);   // Display common driver normal
// lcdSpi (0xC8);   // Display common driver flipped
   
   lcdSpi (0x80 | 16); // Electronic volume to 16
   
   lcdClr ();

   // v--- Likely these aren't needed...And might not be working :P ---v

   lcdSpi (0xef);   // Set refresh rate
   lcdSpi (3);         // 65 Hz
   
   lcdSpi (0x3d);   // Set Charge Pump multiply factor
   lcdSpi (0);         // 5x
   
   lcdSpi (0x36); // Bias ratio 1/4
   
   lcdSpi (0xad);   // set contrast
   lcdSpi (0x20 | 20);      // 20/32
   
   lcdSpi (0xe1);
   lcdSpi (0);
   
   lcdSpi (0xa6);   // Display normal
// lcdSpi (0xA7);   // Display reversed (inverse video)
}


void lcdClr (void)
{
   int i;

   lcdGotoRC (0, 0);

   for (i = 0; i < 16 * 6 * 9; i++) {
      lcdData (0x00); // fill DDRAM with Zeros       
   }
}


void lcdGotoRC (int r, int c)
{
   lcdSpi (0xB0 | (r & 0x0F)); // Set page address to 'r'
   lcdSpi (0x10 | (c >> 4));   // Sets DDRAM column addr - upper 3-bit
   lcdSpi (0x00 | (c & 0x0F)); // lower 4-bit
}


/* lcdData --- send a data byte to the LCD by fast hardware SPI */

inline void lcdData (unsigned char d)
{
  // Data bytes are distinguished from command bytes by an initial
  // '1' bit (followed by 8 data bits). AVR SPI hardware cannot do 9-bit
  // transfers, so we do the initial bit by bit-banging, then switch on
  // the SPI hardware and send a byte directly from the SPDR register.
  // This method is about twice as fast as the software method and
  // gives us a complete screen update in under 4 milliseconds.
  char i;
   
  LCDOUT &= ~CS;
  
  LCDOUT |= SDA;     // Leading '1' bit for LCD data
  LCDOUT &= ~SCLK;   // Toggle the clock
  LCDOUT |= SCLK;
  
#ifdef SOFTWARE_SPI
  for (i = 0; i < 8; i++) {
    if (d & 0x80)
      LCDOUT |= SDA;
    else
      LCDOUT &= ~SDA;

    LCDOUT &= ~SCLK;  // Toggle the clock
    LCDOUT |= SCLK;

    d <<= 1;
  }
#else
  SPCR |= 1 << SPE;
  
  SPDR = d;
  
  while (!(SPSR & (1 << SPIF)))
    ;
    
  SPCR &= ~(1 << SPE);
#endif
   
  LCDOUT |= CS;
}


/* lcdSpi --- send a command byte to the LCD by bit-banging SPI */

inline void lcdSpi (int d)
{
  char i;
   
  LCDOUT &= ~CS;
  // digitalWrite (slaveSelectPin, LOW);
   
  for (i = 0; i < 9; i++) {
    if (d & 0x100)
      LCDOUT |= SDA;
      // digitalWrite (SDAPin, HIGH);
    else
      LCDOUT &= ~SDA;
      // digitalWrite (SDAPin, LOW);

    LCDOUT &= ~SCLK;
    // digitalWrite (SCLKPin, LOW);
    LCDOUT |= SCLK;
    // digitalWrite (SCLKPin, HIGH);

    d <<= 1;
  }
   
  LCDOUT |= CS;
  // digitalWrite (slaveSelectPin, HIGH);
}

