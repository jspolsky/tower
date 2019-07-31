#ifndef _TOWER_H_
#define _TOWER_H_

#include <Arduino.h>
#include <stdarg.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <math.h>
#include "util.h"
// UNDONE - this can be replaced with something leaner
#include "IRLibAll.h"


// IR commands:

#define IR_STANDBY      0xFFE21D
#define IR_FULL_ON      0xFF22DD
#define IR_FADE         0xFF02FD
#define IR_DIMMER_PLUS  0xFFC23D
#define IR_STROBE       0xFFE01F
#define IR_COLOR        0xFFA857
#define IR_DIMMER_MINUS 0xFF906F
#define IR_1            0xFF6897
#define IR_2            0xFF9867
#define IR_3            0xFFB04F
#define IR_4            0xFF30CF
#define IR_5            0xFF18E7
#define IR_6            0xFF7A85
#define IR_7            0xFF10EF
#define IR_8            0xFF38C7
#define IR_9            0xFF5AA5
#define IR_SOUND_ON     0xFF42BD
#define IR_SHOW_0       0xFF4AB5
#define IR_SOUND_OFF    0xFF52AD
#define IR_REPEAT       0xFFFFFFFF


void SlowHueFade(void);
void QuickHueFade(void);
void ShootUp(void);
void Rainbow(void);
void SineWave(void);
void Rotate(void);
void Multihue(void);




#endif
