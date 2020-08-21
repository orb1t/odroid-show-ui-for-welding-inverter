#ifndef GLOBALS_H_
#define GLOBALS_H_



#include <SPI.h>
#include <Wire.h>

#include <Adafruit_ILI9340.h>
#include <Adafruit_GFX.h>
#include "Universum_TFTColours.h" // Universum library for colours
#include "TimerOne.h"

//#include "Universum_TFTColours.h" // Universum library for colours
//#include <dht.h>



#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

//Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// These are the pins used for the UNO
// for Due/Mega/Leonardo use the hardware SPI pins (which are different)
#define _sclk 13
#define _miso 12
#define _mosi 11
#define _cs 10
#define _dc 9
#define _rst 8
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

const int SCR_W = 320;
const int SCR_H = 240;
const int SCR_CX = SCR_W / 2;
const int SCR_CY = SCR_H / 2;



int incrementation = 24;

int scale1 = 1;
int scale2 = 2;
int scale4 = 4;
int scale8 = 8;

uint8_t ledPin = 5; // PWM LED Backlight control to digital pin 5
uint8_t pwm = 255;
uint8_t textSize = 2;
uint8_t rotation = 1;
uint16_t foregroundColor, backgroundColor;

uint32_t imgsize = 0;
uint32_t sizecnt = 0;

void plotLine(int x0, int y0, int x1, int y1, uint16_t color){
    int dx =  abs( x1 - x0 );
    int sx = ( x0 < x1 ? 1 : -1 );
    int dy = -abs( y1 - y0 );
    int sy = ( y0 < y1 ? 1 : -1 );
    int err = dx + dy;  /* error value e_xy */
    while (true) {  /* loop */
//        plot(x0, y0);
        tft.drawFastHLine( x0, y0, 1, color);
        if ( (x0 == x1 ) && ( y0 == y1 ) ) 
          break;
        int e2 = 2*err;
        if (e2 >= dy){ /* e_xy+e_x > 0 */
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {/* e_xy+e_y < 0 */
            err += dx;
            y0 += sy;
        }
    }
}

#ifdef _TESTLINES_
unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9340_BLACK);
  yield();
  
  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) plotLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) plotLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  yield();
  tft.fillScreen(ILI9340_BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) plotLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) plotLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ILI9340_BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) plotLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) plotLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  yield();
  tft.fillScreen(ILI9340_BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) plotLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) plotLine(x1, y1, x2, y2, color);

  yield();
  return micros() - start;
}
#endif  // _TESTLINES_



void initPins()
{
  pinMode(ledPin, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  analogWrite(ledPin, pwm);
}


int getRnd(int v){
  signed int t = random(-5, 20);
  Serial.println(v + t);
  return v + t;//(  t > 0 ? --v : ++v );
}

int vala = 0;
int valb = 0;

#endif  // GLOBALS_H_
