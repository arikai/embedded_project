#ifndef __SOUND_H__
#define __SOUND_H__

#pragma once

#include "common.h"

#include <stdint.h>

extern XDATA(uint16_t) note_table[72];

/* Special values for pitch value */
#define NOTE_REST -1
#define NOTE_TRACK_END -2

/* Note range: A1 - G7S */
#define MIN_PITCH A1
#define MAX_PITCH G7S

// Macros for convenient notation
#define A1 0

#define C4 26
#define D4 28
#define E4 30
#define F4 31
#define G4 33
#define A4 35
#define B4 37

#define G7S 71

#define DEFAULT_BPM 120
#define DEFAULT_SIGNATURE_UPPER 4
#define DEFAULT_SIGNATURE_LOWER 4

// in 64ths - 1
#define NOTE_1               63 // Whole
#define NOTE_2d              47 // Dotted half
#define NOTE_2               31 // Half
#define NOTE_4d              23 // Dotted quarter
#define NOTE_4               15 // Quarter
#define NOTE_8d              11 // Eighth
#define NOTE_8                7 // Eighth
#define NOTE_16d              5 // Sixteenth
#define NOTE_16               3 // Sixteenth
#define NOTE_32d              2 // Thirty-second
#define NOTE_32               1 // Thirty-second
#define NOTE_64               0 // Sixty-fourth

/*
 * Track Macros
 */
#define TRACK_NO_REPEAT   0
#define TRACK_INF_REPEAT -1

// Macros for short track definition
#define N(p, d) { p, ART_NONE,     NOTE_##d },
#define S(p, d) { p, ART_STACCATO, NOTE_##d },
#define L(p, d) { p, ART_LEGATO,   NOTE_##d },
#define R(d)    { NOTE_REST, ART_NONE, NOTE_##d },
#define END     { NOTE_TRACK_END }

enum note_articulation {
    ART_NONE,
    ART_STACCATO,
    ART_LEGATO
};

struct note
{
    int8_t pitch_idx;             // Positive - note_table index. Negative - special value
    uint8_t articulation    : 2; // None, Staccato or Legato
    uint8_t duration_64ths  : 6; // Duration of a note in 64th notes

    // uint16_t freq;
    // uint16_t duration;
};

struct track
{
    uint16_t bpm;
    uint8_t signature_upper;
    uint8_t signature_lower;

    int8_t repeat; // how many times to repeat. -1 - indefinitely
    
    struct note notes[];
};

/* Timer values */
extern uint8_t note_period[2];

void snd_set_bpm(uint16_t bpm);
void snd_set_time_signature(uint8_t beats_per_bar, uint8_t beat_value);
void snd_int_handler(void);
void snd_play_note(struct note*);
void snd_play_track(struct track*);
void snd_set_bg_track(struct track* track);

#endif /* __SOUND_H__ */
