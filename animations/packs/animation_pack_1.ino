#include "FastLED.h"

#define LED_COUNT 100      //Length of your LED strip
#define MAX_INT_VALUE 65536
#define LED_PIN 13 // hardware SPI pin SCK
#define NUM_LEDS 100
#define COLOR_ORDER BGR
#define LED_TYPE WS2811
#define MAX_BRIGHTNESS 255 // watch the power!

uint16_t frame = 0;     //I think I might be able to move this variable to the void loop() scope and save some CPU
uint16_t animateSpeed = 400;            //Number of frames to increment per loop
uint8_t  animation = 9;    //Active animation
uint8_t brightness = 100;    //Global brightness percentage

CRGB leds[NUM_LEDS];

void setup() { 
  LEDS.addLeds<WS2811, LED_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(75);
}

void loop()
{


  //Each animation adjusts the "targetStrip" specified in its parameter.
  //Animations are a function of the current animation frame "frame"
  //Once you've applied the animation to the current strip/frame, Its up to the main loop to send the data to the strip(s)
  
  switch(animation)
  {
  case 0:
    RingPair(leds, frame); 
    break;
  case 1:
    DoubleChaser(leds,frame);
    break;
  case 2:
    TripleBounce(leds,frame);
    break;
  case 3:
    WaveInt(leds,frame,180);
    break;
  case 4:  
    Wave(leds,frame,180);
    break;
  case 5:  //Blue spark (Slow)
    Spark(leds,frame,255,188);   //Overloaded version of "Spark" with Hue value, 255 for fade is the slowest fade possible. 256 is on/off
    delay(2);       //Slow things down a bit more for Slow Spark
    break;
  case 6: //Blue spark (fast)
    Spark(leds,frame,246,188);   //Overloaded version of "Spark" with Hue value, 246 fade is faster which makes for a sharper dropoff
    break;
  case 7:  //White spark (Slow)
    Spark(leds,frame,255);     //"Spark" function without hue make a white spark, 255 for fade is the slowest fade possible.
    delay(2);       //Slow things down a bit more for Slow Spark
    break;
  case 8: //White spark (fast)      //"Spark" function without hue make a white spark, 246 fade is faster which makes for a sharper dropoff
    Spark(leds,frame,245);
    break;
  case 9:
    RainbowSpark(leds,frame,240);    //240 for dropoff is a pretty sharp fade, good for this animation
    break;
   default:
    delay(100);           //Animation OFF
  }
  
  FastLED.show();         //All animations are applied!..send the results to the strip(s)
  frame += animateSpeed;
}


//#######################################################################################################
//##                                      AGGREGATE ANIMATIONS                                         ##
//#######################################################################################################
void TripleBounce(CRGB strip[], uint16_t frame)   //3 chaser animations offset by 120 degrees each
{
  FastLED.clear();    //Clear previous buffer
  Bounce(strip,frame,0);
  Bounce(strip,frame+(MAX_INT_VALUE/3),100);
  Bounce(strip,frame+(MAX_INT_VALUE/3)*2,150);
}

void DoubleChaser(CRGB strip[], uint16_t frame)   //2 chaser animations offset 180 degrees
{
  FastLED.clear();    //Clear previous buffer
  frame = frame * 2;
  Ring(strip, frame, 0);
  Ring(strip, frame + (MAX_INT_VALUE / 2), 150);
}

void RingPair(CRGB strip[], uint16_t frame)     //2 rings animations at inverse phases
{
  FastLED.clear();    //Clear previous buffer
  Ring(strip, frame, 30);
  Ring(strip, MAX_INT_VALUE - frame, 150);
}


void RainbowSpark(CRGB targetStrip[], uint16_t animationFrame,uint8_t fade){    //Color spark where hue is function of frame
  Spark(targetStrip,animationFrame,fade,animationFrame/255);
  delay(20);
}




//#######################################################################################################
//##                                PRIMATIVE ANIMATION FUNCTIONS                                      ##
//#######################################################################################################



//*********************     Bounce      ***************************
// Linear "Larson scanner" (or knight rider effect) with anti-aliasing
// Color is determined by "hue"
//*****************************************************************
void Bounce(CRGB targetStrip[], uint16_t animationFrame, uint8_t hue)
{
  uint16_t pos16;
  if (animationFrame < (MAX_INT_VALUE / 2))
  {
    pos16 = animationFrame * 2;
  
  }else{
    pos16 = MAX_INT_VALUE - ((animationFrame - (MAX_INT_VALUE/2))*2);
  }

  int position = map(pos16, 0, MAX_INT_VALUE, 0, ((LED_COUNT) * 16));
  drawFractionalBar(targetStrip, position, 3, hue,0);
}




//************************          Ring           ******************************
// Anti-aliased cyclical chaser, 3 pixels wide
// Color is determined by "hue"
//*****************************************************
void Ring(CRGB targetStrip[], uint16_t animationFrame, uint8_t hue)
{
  uint8_t stripLength = sizeof(leds)/sizeof(CRGB);
  int pos16 = map(animationFrame, 0, MAX_INT_VALUE, 0, ((stripLength) * 16));
  drawFractionalBar(targetStrip, pos16, 3, hue,1);
}

//***************************   Wave [Float Math]  *******************************
// Squeezed sine wave  
// Uses slow, Arduino sin() function
// Squeezing achieved by using an exponential (^8) sin value
// Color is determined by "hue"
//***********************************************************************************
void Wave(CRGB targetStrip[], uint16_t animationFrame, uint8_t hue){
  FastLED.clear();    //Clear previous buffer
  float deg; 
  float value; 
  uint8_t stripLength = sizeof(leds)/sizeof(CRGB);
  for(uint8_t i=0;i<stripLength;i++)
  {
    deg=float(animationFrame+((MAX_INT_VALUE/stripLength)*i))/(float(MAX_INT_VALUE)) * 360.0;
    value = pow(sin(radians(deg)),8);    //Squeeeeeeze

    if(value>=0){   //Chop sine wave (no negative values)
      targetStrip[i] += CHSV(hue,255,value*256);
    }
  } 
}


//***************************   Wave [Integer Math]  *******************************
// unadulterated sine wave.  
// Uses FastLED sin16() and no float math for efficiency. 
// Since im stuck with integer values, exponential wave-forming is not possible (unless i'm wrong???)
// Color is determined by "hue"
//***********************************************************************************
void WaveInt(CRGB targetStrip[], uint16_t animationFrame, uint8_t hue){
  FastLED.clear();
  uint8_t stripLength = sizeof(leds)/sizeof(CRGB);
  uint8_t value;
  for(uint8_t i=0;i<stripLength;i++)
  {
    value=(sin16(animationFrame+((MAX_INT_VALUE/stripLength)*i)) + (MAX_INT_VALUE/2))/256;   
    if(value>=0){   
      targetStrip[i] += CHSV(hue,255,value);
    }
  } 
}


//********************************   Color Spark  ***********************************
// Color of the sparks is determined by "hue"
// Frequency of sparks is determined by global var "animateSpeed"
// "animateSpeed" var contrained from 1 - 255 (0.4% - 100%)
// "fade" parameter specifies dropoff (next frame brightness = current frame brightness * (x/256)
// fade = 256 means no dropoff, pixels are on or off
// NOTE: this animation doesnt clear the previous buffer because the fade/dropoff is a function of the previous LED state
//***********************************************************************************
void Spark(CRGB targetStrip[], uint16_t animationFrame,uint8_t fade, uint8_t hue){

  uint8_t stripLength = sizeof(leds)/sizeof(CRGB);
  uint16_t rand = random16();

    for(int i=0;i<stripLength;i++)
    {   
      targetStrip[i].nscale8(fade);
    }


  if(rand < (MAX_INT_VALUE / (256 - (constrain(animateSpeed,1,256)))))  ;
  {
    targetStrip[rand % stripLength].setHSV(hue,255,255);
  }
}


//******************************       Spark       **********************************
// Same as color spark but no hue value, // in HSV white is any hue with 0 saturation
// Frequency of sparks is a percentage mapped to global var "animateSpeed"
// "animateSpeed" var contrained from 1 - 255 (0.4% - 100%)
// "fade" parameter specifies dropoff (next frame brightness = current frame brightness * (x/256)
// fade = 256 means no dropoff, pixels are on or off
// NOTE: this animation doesnt clear the previous buffer because the fade/dropoff is a function of the previous LED state
//***********************************************************************************
void Spark(CRGB targetStrip[], uint16_t animationFrame,uint8_t fade){
  uint8_t stripLength = sizeof(leds)/sizeof(CRGB);
  uint16_t rand = random16();
  
  for(int i=0;i<stripLength;i++)
    {   
      targetStrip[i].nscale8(fade);
    }
  

  if(rand < (MAX_INT_VALUE / (256 - (constrain(animateSpeed,1,255)))))
  {
    targetStrip[rand % stripLength].setHSV(0,0,255);
  }

}



//Anti-aliasing code care of Mark Kriegsman Google+: https://plus.google.com/112916219338292742137/posts/2VYNQgD38Pw
void drawFractionalBar(CRGB targetStrip[], int pos16, int width, uint8_t hue, bool wrap)
{
  uint8_t stripLength = sizeof(leds)/sizeof(CRGB);
  uint8_t i = pos16 / 16; // convert from pos to raw pixel number

  uint8_t frac = pos16 & 0x0F; // extract the 'factional' part of the position
  uint8_t firstpixelbrightness = 255 - (frac * 16);
  
  uint8_t lastpixelbrightness = 255 - firstpixelbrightness;

  uint8_t bright;
  for (int n = 0; n <= width; n++) {
    if (n == 0) {
      // first pixel in the bar
      bright = firstpixelbrightness;
    }
    else if (n == width) {
      // last pixel in the bar
      bright = lastpixelbrightness;
    }
    else {
      // middle pixels
      bright = 255;
    }

    targetStrip[i] += CHSV(hue, 255, bright );
    i++;
    if (i == stripLength)
    {
      if (wrap == 1) {
        i = 0; // wrap around
      }
      else{
        return;
      }
    }
  }
}
