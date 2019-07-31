#include "tower.h"


#define PIN_LED_STRIP_1 5
#define PIN_LED_STRIP_2 6
#define PIN_LED_STRIP_3 12
#define PIN_LED_STRIP_4 9
#define PIN_IR_RECEIVER 4
#define COLOR_ORDER RGB
#define CHIPSET     WS2812B
#define NUM_LEDS    450
#define NUM_COLORS 7

static CRGB pride_colors_rgb[NUM_COLORS] = { CRGB(118, 0, 137), CRGB(0, 68, 255), CRGB(0, 129, 31), 
                                    CRGB(255, 239, 0), CRGB(255, 140, 0),  CRGB(231, 0, 0), CRGB(255, 255, 255) }; 

IRrecvPCI myReceiver(PIN_IR_RECEIVER); // IR receiver
IRdecode myDecoder;                    // IR decoder
CRGB leds[4][NUM_LEDS];                // 4 LED strips. Numbered 0-3 internally but 1-4 externally.
uint8_t brightness = 255;
uint8_t color_index = 0;
bool standby = false;
uint8_t blinky_lights_at_top = 1;     // 0 - no blinky lights 
                                      // 1 - yes blinky lights, they are on now 2- yes blinky lights, they are off now

typedef void (*LOOPFUNC)(void);
LOOPFUNC loopfunc;

void setup() 
{

  loopfunc = &SineWave;

  Serial.begin(250000);
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
     
      if (command == IR_REPEAT && repeat_meaning && repeat_meaning != IR_STANDBY && repeat_meaning != IR_STROBE && repeat_meaning != IR_COLOR) 
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
        case IR_STROBE:       if (blinky_lights_at_top) 
                                blinky_lights_at_top = 0;
                              else
                                blinky_lights_at_top = 1;  
                              break;
        case IR_COLOR:        color_index = (color_index + 1) % NUM_COLORS;  break;
        
        case IR_1:            loopfunc = &SlowHueFade;  break;
        case IR_2:            loopfunc = &QuickHueFade; break;
        case IR_3:            loopfunc = &Rainbow;  break;
        case IR_4:            loopfunc = &ShootUp;  break;
        case IR_5:            loopfunc = &SineWave;  break;
        case IR_6:            loopfunc = &Rotate;  break;
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

  if (blinky_lights_at_top)
  {
    EVERY_N_SECONDS(1)
    {
      if (blinky_lights_at_top == 1)
      {
        blinky_lights_at_top = 2;      
      }
      else
      {
        blinky_lights_at_top = 1;
      }
    }
    
    if (blinky_lights_at_top == 1)
    {
      for (int i = 0; i < 4; i++)
      {
        fill_solid(&(leds[i][NUM_LEDS - 4]), 4, CRGB::Red);
      }
    }
  }

  FastLED.show();
  delay(1);
}

void Rainbow()
{
  for (int x=0; x < 6; x++) {
    for (int i=0; i<4; i++)
    {
      fill_solid(&(leds[i][ (NUM_LEDS / 6) * x ]), NUM_LEDS / 6, pride_colors_rgb[x]);
    }
  }

}



void ShootUp()
{
  static long pos = 0;

  for (int i=0; i<4; i++)
  {
    fill_solid( &(leds[i][0]), NUM_LEDS, CRGB::Black );
    fill_solid(&(leds[i][ pos ]), 50, pride_colors_rgb[color_index]);
  }

  pos += 20;
  
  if (pos > NUM_LEDS - 50)
  {
    pos = 0;
  }
  
}


void QuickHueFade()
{
  static uint8_t hue = 0;
  hue += 3;

  for (int i=0; i < 4; i++)
  {
    fill_solid(&(leds[i][0]), NUM_LEDS, CHSV(hue, 255, 255));
  }
  
}


void SlowHueFade()
{
  static uint8_t hue = 0;
  for (int i=0; i < 4; i++)
  {
    fill_solid(&(leds[i][0]), NUM_LEDS, CHSV(hue++, 255, 255));
  }
  delay(100);
}

void SineWave()
{
  const uint16_t BLOB_HEIGHT = 50;
  static uint16_t theta = 0;

  int16_t sin_theta = scale16( 32767 + sin16_avr( theta ), NUM_LEDS - BLOB_HEIGHT );
  bool direction = cos16( theta ) < 0;
  
  theta+=1000;

  for (uint8_t i = 0; i < 4; i++)
  {
    fill_solid( &(leds[i][0]), NUM_LEDS, CRGB::Black );
    fill_solid( &(leds[i][sin_theta]), BLOB_HEIGHT, pride_colors_rgb[color_index]);
    for (int j = 0; j < BLOB_HEIGHT; j++)
    {
      leds[i][sin_theta + j].fadeLightBy(direction ? j * 256 / BLOB_HEIGHT
                                                   : 255 - j * 256 / BLOB_HEIGHT);
    }
  }

}

static uint8_t rgfade[8] = {0, 64, 128, 192, 224, 240, 248, 252 };

void Rotate()
{
  static uint8_t t = 0; // 0 -> 31 time interval cycle of where the light should be pointed
  CRGB rgbColor;

  EVERY_N_MILLIS(25)
  {
    t = (t + 1) % 32;
  }

  for (int i = 0; i < 4; i++)
  {
    CRGB rgbColor = CRGB::Black;
    
    int peak = i * 8;             // the position where the light should be max brightness
    int dist = abs(t - peak);     // the distance from the peak

    if (i == 0 && t > 24)
      dist = 32 - t;              // special case for the first strip so it wraps around

    if (dist < 8)
    {
      rgbColor = pride_colors_rgb[color_index];
      // dim it by the distance from 8
      rgbColor.fadeLightBy(rgfade[dist]);
    }

    fill_solid(&(leds[i][0]), NUM_LEDS, rgbColor);
  }
}
