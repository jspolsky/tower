#include <FastLED.h>

#define COLOR_ORDER RGB
#define CHIPSET     WS2812B
#define NUM_LEDS    450
#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB leds0[NUM_LEDS];
CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];



int DebugPrintf(char* pszFmt, ... ) {

    int cargs = 0;
    char* pszTmp;

    for (pszTmp = pszFmt; *pszTmp; pszTmp++)  
        if (*pszTmp == '%')  
            cargs++; 

    va_list argv;
    va_start(argv, cargs);

    pszTmp = pszFmt;
    while (*pszTmp)
    {
        if (*pszTmp == '%')
        {
            pszTmp++;
            switch (*pszTmp)
            {
                case 'd':   
                    Serial.print(va_arg(argv, int));    
                    break;

                case 'x':   
                    Serial.print(va_arg(argv, int), HEX);    
                    break;

                case 'l':   
                    Serial.print(va_arg(argv, long));
                    break;

                case 'u':
                    Serial.print(va_arg(argv, unsigned long));
                    break;

                case 'f':
                    Serial.print(va_arg(argv, double));
                    break;

                case 'F':
                    Serial.print(va_arg(argv, double), 8);
                    break;

                case 'c':
                    Serial.print((char) va_arg(argv, int));
                    break;

                case 's':
                    Serial.print(va_arg(argv, char*));
                    break;

                case '%':
                    Serial.print('%');
                    break;

                default:
                    break;
            }
        }
        else if (*pszTmp == '\n')
        {
            Serial.println();
        }
        else
        {
            Serial.print(*pszTmp);
        }

        pszTmp++;
    }
}




void setup() 
{
  
  delay(3000); // ?
  
  FastLED.addLeds<CHIPSET, 5, COLOR_ORDER>(leds0, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 6, COLOR_ORDER>(leds1, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 12, COLOR_ORDER>(leds2, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, 9, COLOR_ORDER>(leds3, NUM_LEDS).setCorrection( TypicalLEDStrip );
  
  FastLED.setBrightness( BRIGHTNESS );

}

void loop()
{
  ShootUp();
}



void ShootUp()
{
  static long pos = 0;
  static uint8_t increment = 3;

  fill_solid( &(leds0[0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds1[0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds2[0]), NUM_LEDS, CRGB::Black );
  fill_solid( &(leds3[0]), NUM_LEDS, CRGB::Black );

  fill_solid(&(leds0[ pos ]), 10, CRGB::Red);
  fill_solid(&(leds1[ pos ]), 10, CRGB::Red);
  fill_solid(&(leds2[ pos ]), 10, CRGB::Red);
  fill_solid(&(leds3[ pos ]), 10, CRGB::Red);

  pos = (pos + increment) % (NUM_LEDS - 10);
  increment++;
  if (pos < 10) increment = 3;
  
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
