#include "tower.h"


IRrecvPCI myReceiver(4); //create receiver and pass pin number
IRdecode myDecoder;   //create decoder

#define COLOR_ORDER RGB
#define CHIPSET     WS2812B
#define NUM_LEDS    450
static uint8_t BRIGHTNESS = 255;
#define FRAMES_PER_SECOND 60

static CRGB pride_colors_rgb[6] = { CRGB(118, 0, 137), CRGB(0, 68, 255), CRGB(0, 129, 31), CRGB(255, 239, 0), CRGB(255, 140, 0),  CRGB(231, 0, 0) }; 

bool gReverseDirection = false;

CRGB leds[4][NUM_LEDS];






void setup() 
{

  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
  
  FastLED.addLeds<CHIPSET, 5, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 6, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 12, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 9, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness( BRIGHTNESS );

}

void loop()
{
  //IR: 
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
    myDecoder.dumpResults(false);  //Now print results. Use false for less detail

    if (myDecoder.protocolNum == NEC) {
      switch(myDecoder.value) {
        case 0xFFC23D:
          if (BRIGHTNESS < 250)
            BRIGHTNESS += 15;
            FastLED.setBrightness(BRIGHTNESS);
            FastLED.show();
          break;

        case 0xFF906F:
          if (BRIGHTNESS > 5)
            BRIGHTNESS -= 15;
            FastLED.setBrightness(BRIGHTNESS);
            FastLED.show();
          break;
        
      }
    }
    
    myReceiver.enableIRIn();      //Restart receiver
  }

  // ShootUp();
  Rainbow(pride_colors_rgb, 6);
}

void Rainbow(CRGB colors[], int number_of_colors)
{
  for (int x=0; x < number_of_colors; x++) {
    for (int i=0; i<4; i++)
    {
      fill_solid(&(leds[i][ (NUM_LEDS / number_of_colors) * x ]), 75, colors[x]);
    }
  }
  FastLED.show();
}



void ShootUp()
{
  static long pos = 0;

  fill_solid( &(leds[0][0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds[1][0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds[2][0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds[3][0]), NUM_LEDS, CRGB::Black );

  fill_solid(&(leds[0][ pos ]), 10, CRGB::Green);
  fill_solid(&(leds[1][ pos ]), 10, CRGB::Green);
  fill_solid(&(leds[2][ pos ]), 10, CRGB::Green);
  fill_solid(&(leds[3][ pos ]), 10, CRGB::Green);

  pos += 10;
  if (pos > NUM_LEDS - 10)
  {
    pos = 0;
  }
  
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
