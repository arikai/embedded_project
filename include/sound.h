#ifndef __SOUND_H__
#define __SOUND_H__

#pragma once

#include "common.h"

#include <stdint.h>

extern XDATA(uint16_t) note_table[72];

/* Special value for rest (pause, silence) */
#define NOTE_REST 0
#define NOTE_TRACK_END 1
/* Note frequency by note index */
#define NOTE(n) note_table[n]
/* Note by relative index to C for octave o */
#define NOTECN(o, n) note_table[3+12*(o-2)+n]

/* Note range: A1 - G7S */
#define NOTE_A1 NOTE(0)
#define NOTE_G7S NOTE(71)

#define NOTE_BPM 120

// in milliseconds
#define NOTE_WHOLE        (240000 / NOTE_BPM)
#define NOTE_HALF         (120000 / NOTE_BPM)
#define NOTE_QUARTER      ( 60000 / NOTE_BPM)
#define NOTE_8TH          ( 30000 / NOTE_BPM)
#define NOTE_16TH         ( 15000 / NOTE_BPM)
#define NOTE_32TH         (  7500 / NOTE_BPM)
#define NOTE_64TH         (  3750 / NOTE_BPM)
#define NOTE_128TH        (  1875 / NOTE_BPM)

struct note
{
    // uint8_t freq_idx;     // index in note_table
    // uint8_t duration_64s; // Duration of a note in 64th notes
    uint16_t freq;
    uint16_t duration;
};

/* Timer values */
extern uint8_t note_period[2];

void set_note_period(uint16_t note_freq);
void set_ena(uint8_t value);
void beep(void);
void play_note(struct note*);
void play_track(struct note[]);

#endif /* __SOUND_H__ */
