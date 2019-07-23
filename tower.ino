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
uint8_t brightness = 255;
bool standby = false;

typedef void (*LOOPFUNC)(void);
LOOPFUNC loopfunc;

void setup() 
{

  loopfunc = &SlowHueFade;

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
  static uint32_t repeat_meaning = 0; 
  
  // IR
  
  if (myReceiver.getResults()) 
  {
    myDecoder.decode();           //Decode it
            //    myDecoder.dumpResults(false);  //Now print results. Use false for less detail

    if (myDecoder.protocolNum == NEC) 
    {

      uint32_t command = myDecoder.value;
     
      if (command == IR_REPEAT && repeat_meaning && repeat_meaning != IR_STANDBY) 
        command = repeat_meaning;
      
      switch(command)
      {
      
        case IR_DIMMER_PLUS:  

          if (brightness < 20)
            brightness++;
          else
            brightness = min(255, brightness + 10);
          FastLED.setBrightness(brightness);
          break;
          
        case IR_DIMMER_MINUS: 

          if (brightness < 20)
            brightness = max(1, brightness - 1);
          else
            brightness = brightness - 10;
          FastLED.setBrightness(brightness);
          break;

        case IR_FULL_ON: 

          brightness = 255;
          FastLED.setBrightness(brightness);
          break;

        case IR_STANDBY:

          standby = !standby;
          FastLED.setBrightness(standby ? 0 : brightness);
          break;

        case IR_FADE:         DebugPrintf("Unimplemented IR_FADE\n");  break;
        case IR_STROBE:       DebugPrintf("Unimplemented IR_STROBE\n");  break;
        case IR_COLOR:        DebugPrintf("Unimplemented IR_COLOR\n");  break;
        
        case IR_1:            loopfunc = &SlowHueFade;  break;
        case IR_2:            loopfunc = &QuickHueFade; break;
        case IR_3:            loopfunc = &Rainbow;  break;
        case IR_4:            loopfunc = &ShootUp;  break;
        case IR_5:            DebugPrintf("Unimplemented IR_5\n");  break;
        case IR_6:            DebugPrintf("Unimplemented IR_6\n");  break;
        case IR_7:            DebugPrintf("Unimplemented IR_7\n");  break;
        case IR_8:            DebugPrintf("Unimplemented IR_8\n");  break;
        case IR_9:            DebugPrintf("Unimplemented IR_9\n");  break;
        
        case IR_SOUND_ON:     DebugPrintf("Unimplemented IR_SOUND_ON\n");  break;
        case IR_SHOW_0:       DebugPrintf("Unimplemented IR_SHOW_0\n");  break;
        case IR_SOUND_OFF:    DebugPrintf("Unimplemented IR_SOUND_OFF\n");  break;

      }
      repeat_meaning = command;
    }
    
    myReceiver.enableIRIn();      //Restart receiver
  }

  (*loopfunc)();
}

void Rainbow()
{
  for (int x=0; x < 6; x++) {
    for (int i=0; i<4; i++)
    {
      fill_solid(&(leds[i][ (NUM_LEDS / 6) * x ]), NUM_LEDS / 6, pride_colors_rgb[x]);
    }
  }
  FastLED.show();
  delay(1);
}



void ShootUp()
{
  static long pos = 0;

  fill_solid( &(leds[0][0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds[1][0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds[2][0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds[3][0]), NUM_LEDS, CRGB::Black );

  fill_solid(&(leds[0][ pos ]), 25, CRGB::Green);
  fill_solid(&(leds[1][ pos ]), 25, CRGB::Green);
  fill_solid(&(leds[2][ pos ]), 25, CRGB::Green);
  fill_solid(&(leds[3][ pos ]), 25, CRGB::Green);

  pos += 10;
  if (pos > NUM_LEDS - 25)
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
