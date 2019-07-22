#include <FastLED.h>

#define COLOR_ORDER RGB
#define CHIPSET     WS2812B
#define NUM_LEDS    450
#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB leds[NUM_LEDS][4];

void setup() 
{
  
  delay(3000); // ?
  
  FastLED.addLeds<CHIPSET, 5, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 6, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 12, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 9, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness( BRIGHTNESS );

}

void loop()
{
  SineWave();
}



void SineWave()
{
  static uint8_t pos = 0;

  fill_solid( &(leds[0][0]), NUM_LEDS*4, CRGB::Black );

  for (int i=0; i<3; i++)
    fill_solid(&(leds[ 0xFF & ((uint16_t) pos * NUM_LEDS / 255) ][i]), 10, CRGB::Red);

  pos += 5;
  
  FastLED.show();
  delay(1);
  
}


void QuickHueFade()
{
  static uint8_t hue = 0;
  hue += 3;
  FastLED.showColor(CHSV(hue, 255, 255)); 
  delay(1);
}


void SlowHueFade()
{
  static uint8_t hue = 0;
  FastLED.showColor(CHSV(hue++, 255, 255)); 
  delay(100);
}
