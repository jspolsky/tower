#include "tower.h"


#define PIN_LED_STRIP_1 5
#define PIN_LED_STRIP_2 6
#define PIN_LED_STRIP_3 12
#define PIN_LED_STRIP_4 9
#define PIN_IR_RECEIVER 4
#define COLOR_ORDER RGB
#define CHIPSET     WS2812B
#define NUM_LEDS    450

static CRGB pride_colors_rgb[6] = { CRGB(118, 0, 137), CRGB(0, 68, 255), CRGB(0, 129, 31), 
                                    CRGB(255, 239, 0), CRGB(255, 140, 0),  CRGB(231, 0, 0) }; 

IRrecvPCI myReceiver(PIN_IR_RECEIVER); // IR receiver
IRdecode myDecoder;                    // IR decoder
CRGB leds[4][NUM_LEDS];                // 4 LED strips. Numbered 0-3 internally but 1-4 externally.
static uint8_t brightness = 255;

void setup() 
{

  Serial.begin(9600);
  delay(2000); 
  myReceiver.enableIRIn(); // Start the receiver

  
  FastLED.addLeds<CHIPSET, PIN_LED_STRIP_1, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_LED_STRIP_2, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_LED_STRIP_3, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, PIN_LED_STRIP_4, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness( brightness );

}

void loop()
{
  //IR: 
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) {
    myDecoder.decode();           //Decode it
//    myDecoder.dumpResults(false);  //Now print results. Use false for less detail

    if (myDecoder.protocolNum == NEC) {
      switch(myDecoder.value) {

        case IR_DIMMER_PLUS:  

          if (brightness < 255)
            brightness += 10;
            FastLED.setBrightness(brightness);
            FastLED.show();
          break;
          
        case IR_DIMMER_MINUS: 

          if (brightness > 10)
            brightness -= 10;
            FastLED.setBrightness(brightness);
            FastLED.show();
          break;

        case IR_STANDBY:      DebugPrintf("Unimplemented IR_STANDBY\n"); break;  
        case IR_FULL_ON:      DebugPrintf("Unimplemented IR_FULL_ON\n"); break; 
        case IR_FADE:         DebugPrintf("Unimplemented IR_FADE\n");  break;
        case IR_STROBE:       DebugPrintf("Unimplemented IR_STROBE\n");  break;
        case IR_COLOR:        DebugPrintf("Unimplemented IR_COLOR\n");  break;
        case IR_1:            DebugPrintf("Unimplemented IR_1\n");  break;
        case IR_2:            DebugPrintf("Unimplemented IR_2\n");  break;
        case IR_3:            DebugPrintf("Unimplemented IR_3\n");  break;
        case IR_4:            DebugPrintf("Unimplemented IR_4\n");  break;
        case IR_5:            DebugPrintf("Unimplemented IR_5\n");  break;
        case IR_6:            DebugPrintf("Unimplemented IR_6\n");  break;
        case IR_7:            DebugPrintf("Unimplemented IR_7\n");  break;
        case IR_8:            DebugPrintf("Unimplemented IR_8\n");  break;
        case IR_9:            DebugPrintf("Unimplemented IR_9\n");  break;
        case IR_SOUND_ON:     DebugPrintf("Unimplemented IR_SOUND_ON\n");  break;
        case IR_SHOW_0:       DebugPrintf("Unimplemented IR_SHOW_0\n");  break;
        case IR_SOUND_OFF:    DebugPrintf("Unimplemented IR_SOUND_OFF\n");  break;
        case IR_REPEAT:       DebugPrintf("Unimplemented IR_REPEAT\n");  break;
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
