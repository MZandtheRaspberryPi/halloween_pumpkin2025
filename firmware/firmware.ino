#include <FastLED.h>
#define NUM_LEDS 5
#define DATA_PIN 3
#define FRAMES_PER_SECOND 60   // how fast should the led animation render
#define BRIGHTNESS  255        // sets the overall brightness of the leds
#define MIN_HEAT 10
#define MAX_HEAT 150
#define HEAT_CHANGE_INCR 2
#define SECONDS_PER_COLOR_STEP 5

CRGB leds[NUM_LEDS];
 
CRGBPalette16 currentPalette = HeatColors_p; // sets a variable for CRGBPalette16 which allows us to change this value later

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS); 
  FastLED.setBrightness( BRIGHTNESS );   // sets the brightness to the predetermined levels
}
 
void loop() {

  update_colors(); // calls the function defined below this loop & runs simulation frames, using palette colors

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND); // sets a delay using the number 1000 devided by the predetermined frames per second.
}

void update_colors()   // defines a new function
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];
  static byte cur_heat = 150;
  static bool going_down = true;
  static unsigned long last_color_update = 0;

  if ((millis() - last_color_update) > SECONDS_PER_COLOR_STEP * 1000)
  {
    last_color_update = millis();
     if (going_down)
      {
        if (cur_heat < MIN_HEAT)
        {
          going_down = !going_down;
        }
        else
        {
          cur_heat -= HEAT_CHANGE_INCR;
        }
      }
      else {
        if (cur_heat > MAX_HEAT)
        {
          going_down = ! going_down;
        }
        else {
          cur_heat += HEAT_CHANGE_INCR;
        }
      }
  }

  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( cur_heat, 248);
    leds[j] = ColorFromPalette( currentPalette, colorindex);
  }
}
