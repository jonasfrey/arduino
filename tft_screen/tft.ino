/*
  Simple "Hello World" for ILI9341 LCD

  https://wokwi.com/arduino/projects/308024602434470466
*/

// https://wokwi.com/projects/364286042641552385

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

struct O_text
{
    char * s_text;
    char * s_text__last;
    int n_cursor_x;
    int n_cursor_x__last;
    int n_cursor_y;
    int n_cursor_y__last;
    int n_color;
    int n_color__last;
    int n_text_size;
    int n_text_size__last;
};


void f_init_o_text(
    O_text * o_text,
    char * s_text,
    int n_cursor_x,
    int n_cursor_y,
    int n_color,
    int n_text_size
){
    o_text->s_text = s_text;
    o_text->s_text__last = s_text;
    o_text->n_cursor_x = n_cursor_x;
    o_text->n_cursor_x__last = n_cursor_x;
    o_text->n_cursor_y = n_cursor_y;
    o_text->n_cursor_y__last = n_cursor_y;
    o_text->n_color = n_color;
    o_text->n_color__last = n_color;
    o_text->n_text_size = n_text_size;
    o_text->n_text_size__last = n_text_size;

}
void f_update_o_text(
    O_text * o_text,
    int n_clear_color
){
    if(
        // strcmp(o_text->s_text, o_text->s_text__last)
        // // o_text->s_text != o_text->s_text__last
        //     || 
        // o_text->n_cursor_x != o_text->n_cursor_x__last
        //     || 
        // o_text->n_cursor_y != o_text->n_cursor_y__last
        //     || 
        // o_text->n_color != o_text->n_color__last
        //     || 
        // o_text->n_text_size != o_text->n_text_size__last
        1
    ){
        //__last
        tft.setCursor(
            o_text->n_cursor_x__last,
            o_text->n_cursor_y__last
        );
        tft.setTextColor(
            n_clear_color
        );
        tft.setTextSize(
            o_text->n_text_size__last
        );
        tft.println(
            o_text->s_text__last
        );

        //new
        tft.setCursor(
            o_text->n_cursor_x,
            o_text->n_cursor_y
        );
        tft.setTextColor(
            o_text->n_color
        );
        tft.setTextSize(
            o_text->n_text_size
        );
        tft.println(
            o_text->s_text
        );

        o_text->s_text__last = o_text->s_text;
        o_text->n_cursor_x__last = o_text->n_cursor_x;
        o_text->n_cursor_y__last = o_text->n_cursor_y;
        o_text->n_color__last = o_text->n_color;
        o_text->n_text_size__last = o_text->n_text_size;
    };
}

struct O_text o_text;
struct O_text o_text2;
char s_text[200] = "test";
char s_text2[200] = "o";

int n_color_background = ILI9341_BLACK;
void setup() {

Serial.begin(9600);
  tft.begin();

  tft.setCursor(26, 120);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.println("Hello, TFT!");

  tft.setCursor(20, 160);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.println("I can has colors?");

  tft.fillScreen(n_color_background);

    f_init_o_text(
        &o_text,
        s_text, //char * s_text
        0, //int n_cursor_x,
        0, //int n_cursor_y,
        0xff00, //int n_color,
        3 //int n_text_size,
    );
    
    
    f_init_o_text(
        &o_text2,
        s_text2, //char * s_text
        0, //int n_cursor_x,
        0, //int n_cursor_y,
        ILI9341_RED, //int n_color,
        2 //int n_text_size,
    );
  // Meme reference: https://english.stackexchange.com/questions/20356/origin-of-i-can-haz
}

char str[200];
int n_frame = 0;
void loop() {
    n_frame+=1;
    sprintf(str, "micros: %lu", micros());
    strcpy(o_text.s_text, str);
    f_update_o_text(&o_text, n_color_background);

    // o_text2.n_color = ILI9341_RED; 
    strcpy(o_text2.s_text, str);

    sprintf(str, "%c", (n_frame % 64) + 64);
    strcpy(o_text2.s_text, str);
    Serial.println(str);

    o_text2.n_cursor_x = (int)(( sin(0.1*((float)n_frame))*.5 + 0.5)*72.0);
    o_text2.n_cursor_y = (int)(( cos(0.3*((float)n_frame))*.5 + 0.5)*72.0);
    f_update_o_text(&o_text2, n_color_background);

    delay(1000);
}