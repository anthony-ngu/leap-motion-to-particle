#include "neopixel/neopixel.h"

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN A5
#define PIXEL_COUNT 5
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
uint32_t forceColor = strip.Color(0,0,255); // Keeps track of the "force" color

void setup() 
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(10); // Stops the LEDs from being blinding
  fillFrom(0, strip.numPixels(), strip.Color(255,255,255)); // Shows that it is working by putting all LEDs to white
  strip.show();
  
  Particle.function("setStrength", setStrength);
  Particle.function("turnOff", turnOff);
  Particle.function("selectForceColor", selectForceColor);
}
void loop() 
{
}

int selectForceColor(String args)
{
    int colorIndex = 0;
    if(args == "")
    {
        colorIndex = random(5);
    }else{
        colorIndex = args.toInt();
    }
    
    switch(colorIndex)
    {
        case 0:
            // blue
            forceColor = strip.Color(0,0,255);
            break;
        case 1:
            // purple
            forceColor = strip.Color(148,0,211);
            break;
        case 2:
            // red
            forceColor = strip.Color(255,0,0);
            break;
        case 3:
            // orange
            forceColor = strip.Color(255,165,0);
            break;
        default:
            // green
            forceColor = strip.Color(0,255,0);
            break;
    }
    return colorIndex;
}

int setStrength (String args)
{
    double strengthLevel = args.toInt() / 100.0;
    fillFrom(0, strip.numPixels() * strengthLevel, forceColor);
    fillFrom(strip.numPixels() * strengthLevel + 1, strip.numPixels(), strip.Color(0, 0, 0));
    strip.show();
    return 1;
}

int turnOff(String args)
{
    fillFrom(0, strip.numPixels(), strip.Color(0,0,0));
    strip.show();
    return 1;
}


// Fills from the start pixel, to the finish pixel with the color
void fillFrom(uint16_t start, uint16_t finish, uint32_t color)
{
    uint16_t i;
    for(i = start; i<=finish; i++)
    {
        strip.setPixelColor(i, color);
    }
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
