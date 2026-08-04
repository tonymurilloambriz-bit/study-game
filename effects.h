#include <ArduboyTones.h>

// SFX: direction=down length=2 speed=33 pitch=48 range=14
// Example: sound.tones(hit);
const uint16_t hit[] PROGMEM = {
  NOTE_E5H, 60, NOTE_REST, 2, NOTE_B3H, 34,
  TONES_END
};

// SFX: direction=valley length=4 speed=28 pitch=72 range=8 vibe=0 grit=0 curve=0 attack=1.1 shape=0 repeat=1 seed=2439267779
// Example: sound.tones(blip);
const uint16_t blip[] PROGMEM = {
  NOTE_GS6H, 27,  NOTE_REST, 2,  NOTE_DS6H, 28,  NOTE_REST, 2,
  NOTE_DS6H, 31,  NOTE_REST, 2,  NOTE_GS6H, 27,
  TONES_END
};
