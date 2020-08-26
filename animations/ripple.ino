#include <FastLED.h>

#define NUM_LEDS    100
#define DATA_PIN    13
#define CHIPSET     WS2811
#define COLOUR_CODE BGR

int delayTime = 25;
int hue = 0;
CRGB leds[NUM_LEDS];

void setup()
{
  LEDS.addLeds<WS2811,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
}

void fadeall() 
{
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i].nscale8(254);  //for CYCLon
  }
}

void loop() 
{
  int rip1 = 50;
  int rip2 = 50;
  
  for (int counter = 0; counter < 50; counter++)
  {
    rip1++; rip2--;
    leds[rip1] = CRGB::Green;
    leds[rip2] = CRGB::Green;
    FastLED.show();
    leds[rip1] = CRGB::White;
    leds[rip2] = CRGB::White;
    delay(30);
  }
}
