#include <FastLED.h>

#define NUM_LEDS    100
#define DATA_PIN    13
#define CHIPSET     WS2811
#define COLOUR_CODE BGR
 
int delayTime = 10;
int hue = 0;
CRGB leds[NUM_LEDS];

void setup()
{
  LEDS.addLeds<WS2811,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(120);
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(255);  //for CYCLon
  }
}

void loop() 
{
  for(int dot = 0; dot < NUM_LEDS; dot++) { 
    leds[dot] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(delayTime);
  }
  for(int dot = NUM_LEDS; dot > 0; dot--) { 
    leds[dot] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(delayTime);
  }
  
}
