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
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct O_hsl
{
	int h;
	float s;
	float l;
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

struct o_rgb f_o_rgb_from_o_hsl(struct O_hsl o_hsl) {
	struct O_rgb o_rgb;

	if (o_hsl.s == 0)
	{
		o_rgb.r = o_rgb.g = o_rgb.b = (unsigned char)(o_hsl.l * 255);
	}
	else
	{
		float v1, v2;
		float hue = (float)o_hsl.H / 360;

		v2 = (o_hsl.l < 0.5) ? (o_hsl.l * (1 + o_hsl.s)) : ((o_hsl.l + o_hsl.s) - (o_hsl.l * o_hsl.s));
		v1 = 2 * o_hsl.l - v2;

		o_rgb.r = (unsigned char)(255 * f_n_rgb_from_hue(v1, v2, hue + (1.0f / 3)));
		o_rgb.g = (unsigned char)(255 * f_n_rgb_from_hue(v1, v2, hue));
		o_rgb.b = (unsigned char)(255 * f_n_rgb_from_hue(v1, v2, hue - (1.0f / 3)));
	}

	return o_rgb;
}

void setup() {
  tft.begin();

  tft.setCursor(0, 0);

  int n_scl_x = 72;
  for(int n = 0; n< n_scl_x ; n+=1){
    struct HSL data = { 138, 0.50f, 0.76f };
    struct RGB value = HSLToRGB(data);
    int n_col = 
    tft.drawPixel(n, 0, n);

  }

  // tft.setTextSize(1);
  // tft.setTextColor(ILI9341_RED);
  // tft.println("Hello, TFT!");

  // Meme reference: https://english.stackexchange.com/questions/20356/origin-of-i-can-haz
}

void loop() { }