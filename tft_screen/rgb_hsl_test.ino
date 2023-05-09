/*
  Simple "Hello World" for ILI9341 LCD

  https://wokwi.com/arduino/projects/308024602434470466
*/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

struct O_rgb
{
  unsigned char n_r;
  unsigned char n_g;
  unsigned char n_b;
};

struct O_hsl
{
  int n_h;
  float n_s;
  float n_l;
};

float f_n_rgb_from_hue(float v1, float v2, float vH)
{
  if (vH < 0)
    vH += 1;

  if (vH > 1)
    vH -= 1;

  if ((6 * vH) < 1)
    return (v1 + (v2 - v1) * 6 * vH);

  if ((2 * vH) < 1)
    return v2;

  if ((3 * vH) < 2)
    return (v1 + (v2 - v1) * ((2.0f / 3) - vH) * 6);

  return v1;
}

struct O_rgb f_o_rgb_from_o_hsl(struct O_hsl o_hsl) {
  struct O_rgb o_rgb;

  if (o_hsl.n_s == 0)
  {
    o_rgb.n_r = o_rgb.n_g = o_rgb.n_b = (unsigned char)(o_hsl.n_l * 255);
  }
  else
  {
    float v1, v2;
    float n_hue = o_hsl.n_h ;

    v2 = (o_hsl.n_l < 0.5) ? (o_hsl.n_l * (1 + o_hsl.n_s)) : ((o_hsl.n_l + o_hsl.n_s) - (o_hsl.n_l * o_hsl.n_s));
    v1 = 2 * o_hsl.n_l - v2;

    o_rgb.n_r = (unsigned char)(255 * f_n_rgb_from_hue(v1, v2, n_hue + (1.0f / 3)));
    o_rgb.n_g = (unsigned char)(255 * f_n_rgb_from_hue(v1, v2, n_hue));
    o_rgb.n_b = (unsigned char)(255 * f_n_rgb_from_hue(v1, v2, n_hue - (1.0f / 3)));
  }

  return o_rgb;
}

word f_n_565colformat_from_rgb( byte R, byte G, byte B)
{
  // convert RGB(255,255,255) to 565 format (FFFF)
  return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );
}

void setup() {
  tft.begin();

  tft.setCursor(0, 0);
  Serial.begin(115200);
  int n_scl_x = 250;
  for (int n = 0; n < n_scl_x ; n += 1) {
    float n_nor = ((float)n / (float)n_scl_x);
    struct O_hsl o_hsl = { 0.89, 1.f, .5f };
    struct O_rgb o_rgb = f_o_rgb_from_o_hsl(o_hsl);
    int n_col = 0;
    // int n_r = ((float) o_rgb.n_r / (float)255.0) * (float)((1<<5)-1);
    // int n_g = ((float) o_rgb.n_g / (float)255.0) * (float)((1<<6)-1);
    // int n_b = ((float) o_rgb.n_b / (float)255.0) * (float)((1<<5)-1);
    // n_col = n_col | (n_r << (6+5));
    // n_col = n_col | (n_g << (6));
    // n_col = n_col | (n_b << (0));

    // n_col = f_n_565colformat_from_rgb(n_nor * 255., n_nor * 255., n_nor * 255.);
    n_col = f_n_565colformat_from_rgb(o_rgb.n_r, o_rgb.n_g, o_rgb.n_b);
    tft.drawPixel(n, 0, n_col);
    // Serial.println(o_rgb.n_r);
  }
  delay(100000);
  // tft.setTextSize(1);
  // tft.setTextColor(ILI9341_RED);
  // tft.println("Hello, TFT!");

  // Meme reference: https://english.stackexchange.com/questions/20356/origin-of-i-can-haz
}

void loop() { }
