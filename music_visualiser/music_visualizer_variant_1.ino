#include <FastLED.h>

#define LED_PIN     13
#define NUM_LEDS    100
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 250

int sens_val = 27;
int audio = A0;
int loop_max = 0;
int k = 255;
int decay = 0;
int decay_check = 0;
long pre_react = 0;
long react = 0;
long post_react = 0;
int wheel_speed = 1;

void setup()
{
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);

  for (int i = 0; i < NUM_LEDS; i++) { leds[i] = CRGB(0, 0, 0); }
    
  FastLED.show();

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

void rainbow()
{
  for(int i = NUM_LEDS - 1; i >= 0; i--) {
    if (i < react)
      leds[i] = Scroll((i * 256 / 50 + k) % 256);
    else
      leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show(); 
}

void loop()
{
  int audio_input = analogRead(audio) * sens_val;
  if (audio_input > 0)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 255L;
    if (pre_react > react)
      react = pre_react;
    Serial.print(audio_input);
    Serial.print(" -> ");
    Serial.println(pre_react);
  }

  rainbow();
  
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
