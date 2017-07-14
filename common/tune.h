#pragma once

#include "../common/pin.h"

#define NOTE_PAUSE 1
#define NOTE_E4 330
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_C5S 554
#define NOTE_D5 587

typedef unsigned int Tune[];

void play_tune(Pin pin, Tune tune);
