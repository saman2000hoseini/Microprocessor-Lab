#include "pitches.h"
#include "musics.h"

#define PIEZO 9

const double gap = 1.2;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  playMelody(odeToJoy, odeToJoyDurations, sizeof(odeToJoy) / sizeof(int));
}

void playMelody(int melody[], float durations[], int mLength) {
  for (int index = 0; index < mLength; index++) {
    float scale = analogRead(A0) / 512.0;
    int duration = 700 / durations[index];
    tone(PIEZO, (int)(melody[index]*scale), duration);
    delay(duration * gap);
  }
}
