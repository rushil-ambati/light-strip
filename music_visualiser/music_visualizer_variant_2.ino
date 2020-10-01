#include <FastLED.h>

// LED OBJECT SETUP
#define LED_PIN     4
#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

int audio = A0;

// STANDARD VISUALISER VARIABLES
int k = 255; // COLOR WHEEL POSITION
int decay = 1; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long react = 0; // NUMBER OF LEDs BEING LIT

// RAINBOW WAVE SETTINGS
int wheel_speed = 1;

void setup()
{
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);

  FastLED.show();

  // INPUT SETUP
  pinMode(audio, INPUT);
}

CRGB Scroll(int pos) {
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  return color;
}

void leftripple()
{
    int rip_check = 50;
    for(int ripple = 50; ripple > 0; ripple--)
    {
      if (ripple > (rip_check - react))
      {
        leds[ripple] = Scroll((ripple * 256 / 50 + k) % 256);
      }
      else
      {
        leds[ripple] = CRGB(0, 0, 0);
      }
    }
    rip_check++;
}

void rightripple()
{
    for(int ripple = 50; ripple < 100; ripple++)
    {
      int rip_check = 50;
      if (ripple < (rip_check + react))
      {
        leds[ripple] = Scroll((ripple * 256 / 50 + k) % 256);
      }
      else
      {
        leds[ripple] = CRGB(0, 0, 0);
      }
      rip_check++;
    }
}


void loop()
{
  int audio_input = analogRead(audio) * 5; // SENSITIVITY PARAMETER
  if (audio_input > 0)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 255L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

    if (pre_react > 50 && pre_react < 100) { wheel_speed = 3; }
    else if (pre_react > 100){ wheel_speed = 5; }
    else { wheel_speed = 1; }
    
    /*
    if (pre_react > 50 && pre_react < 100) { wheel_speed = 5; }
    else if (pre_react > 100){ wheel_speed = 10; }
    else { wheel_speed = 3; }
    */
  }

  leftripple();
  rightripple();
  FastLED.show();

  k = k - wheel_speed;
  if (k < 0)
    k = 255;
    
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0)
      react--;
  }
}
