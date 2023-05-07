/*
 * Interfacing Arduino with ST7789 TFT display (240x240 pixel).
 * Graphics test example.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 */
/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library

// ST7789 TFT module connections
#define TFT_CS 10 // define chip select pin
#define TFT_DC 9  // define data/command pin
#define TFT_RST 8 // define reset pin, or set to -1 and connect to Arduino RESET pin

// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void)
{
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));

    // if the display has CS pin try with SPI_MODE0
    tft.init(240, 240, SPI_MODE2); // Init ST7789 display 240x240 pixel
    tft.fillScreen(ST77XX_BLACK);
    // if the screen is flipped, remove this command
    tft.setRotation(2);
    tft.setTextSize(3);
    tft.setTextWrap(true);
}
unsigned long n_last = 0;
char str[256];
void loop()
{

    unsigned long n_now = micros();

    // if we use `tft.fillScreen` to clear the screen 
    // the clearing will be so slow that flickering may appear
    // so to clear anything we have to overwrite the exact pixels with black color

    tft.setCursor(0, 30);
    sprintf(str, "%lu", n_last);
    tft.setTextColor(ST77XX_BLACK);
    // set the last tetx to black
    tft.println(str);
    tft.setCursor(0, 30);

    sprintf(str, "%lu", n_now);
    n_last = n_now;

    tft.setTextColor(ST77XX_GREEN);
    // set the current text to color
    tft.println(str);

    //use a optionally delay to also prevent flickering
    // delay(100);

}