#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 100
#define DATA_PIN 13
#define CHIPSET     WS2811
#define COLOR_ORDER BRG

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  LEDS.addLeds<WS2811,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    // leds[i].nscale8(250);  //for CYCLon
    leds[i].fadeToBlackBy( 15 );
  }
}

void loop() { 
    static uint8_t hue = 0;
    // First slide the led in one direction
    for (int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to red
      leds[i] = CHSV(hue++, 255, 255);
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      // leds[i] = CRGB::Black;
      fadeall();
      // Wait a little bit before we loop around and do it again
      delay(10);
    }
    for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
      // Set the i'th led to red
      leds[i] = CHSV(hue++, 255, 255);
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      // leds[i] = CRGB::Black;
      fadeall();
      // Wait a little bit before we loop around and do it again
      delay(10);
    }
}
