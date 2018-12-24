#include "aduc812.h"
#include "sound.h"
#include "system.h"
#include "time.h"
#include "timer.h"

#include <stdint.h>

#include "led.h" // DEBUG

/* 
 * Python script for frequencies:
 *
 * a0 = 27.5
 * for octave in range(1,8):
 *   print(', '.join('{:4}'.format(int(a0*2**(octave+n/12))) for n in range(0,11)), ',', sep='')
 */

#define MIN_FREQ 55 // Lower values used for special cases
		    // (rest, track end, etc.)

XDATA(uint16_t) pitch_table[] = {
      55,   58,   61,   65,   69,   73,   77,   82,   87,   92,   97,  103,
     110,  116,  123,  130,  138,  146,  155,  164,  174,  184,  195,  207,
     220,  233,  246,  261,  277,  293,  311,  329,  349,  369,  391,  415,
     440,  466,  493,  523,  554,  587,  622,  659,  698,  739,  783,  830,
     880,  932,  987, 1046, 1108, 1174, 1244, 1318, 1396, 1479, 1567, 1661,
    1760, 1864, 1975, 2093, 2217, 2349, 2489, 2637, 2793, 2959, 3135, 3322,
    3520, 3729, 3951, 4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271, 6644,
};

uint8_t note_period[2] = { 0xff, 0xff };


/*
 * Module-level functions
 */

#define DURATION(note) ((whole_note_duration * (note->duration_64ths+1)) >> 6)
void snd_set_note_period(uint16_t note_freq)
{
    uint16_t delay = TIMER_FREQ / (2 * note_freq);
    uint8_t* ptr = (uint8_t*) &delay;
    // sdcc -> little endian? aduc -> big endian?
    note_period[0] = 0xFF - ptr[0];
    note_period[1] = 0xFF - ptr[1];
}


/*
 * API
 */

static uint16_t current_bpm           = DEFAULT_BPM;
static uint8_t current_beats_per_bar  = DEFAULT_SIGNATURE_UPPER;
static uint8_t current_beat_value     = DEFAULT_SIGNATURE_LOWER;

static unsigned long whole_note_duration; // Recalculated in init

void recalculate_whole_note_duration()
{
    whole_note_duration = ((unsigned long) 1000) * (((uint16_t) current_beat_value * 60) / current_bpm);
}

void snd_set_bpm(uint16_t bpm)
{
    current_bpm = bpm;
    recalculate_whole_note_duration();
}

void snd_set_time_signature(uint8_t beats_per_bar, uint8_t beat_value)
{
    current_beats_per_bar = beats_per_bar;
    current_beat_value = beat_value;
    recalculate_whole_note_duration();
}


/*
 * Set background track
 */

static struct track* bg_track;
static unsigned long bg_note_till = 0;
static unsigned long bg_note_till_pause = 0;
static uint16_t bg_note_idx;
static bit nosound = 0;
static int8_t bg_repeat_count = 0;

void snd_int_handler(void)
{
    struct note* note;
    uint16_t note_period;
    int8_t pitch_idx;
    unsigned long note_duration  = 0;
    unsigned long pause_duration = 0;
    unsigned long current_time;

    // Only some revisions of SDK support 3-bit volume (0x1C)
    // Use only 1-bit volume
    if( !nosound )
	xor_ena(0x10);

    if( !bg_track ) return;

    // Even though this block is huge, 
    // usually it doesn't take much time to execute it
    current_time = get_time();
    while( bg_note_till <= current_time )
    {
	// If not Legato, pause after played note must be taken
	if( bg_note_till_pause )
	{
	    bg_note_till = bg_note_till_pause;
	    bg_note_till_pause = 0;
	    nosound = 1;
	    continue; // Check if pause time already passed
	}

	while(1) 
	{
	    ET0 = 0; // Ignore interrupt until we calculate new parameters

	    note = &(bg_track->notes[bg_note_idx++]);
	    pitch_idx = note->pitch_idx;

	    if( pitch_idx == NOTE_TRACK_END )
	    {
		if( bg_repeat_count == 0 )
		{
		    bg_track = 0;
		    nosound = 0;
		    return;
		}
		if( bg_repeat_count > 0 )
		{
		    --bg_repeat_count;
		}
		bg_note_idx = 0;
		continue;
	    }

	    note_duration = DURATION(note);
	    if( pitch_idx == NOTE_REST )
	    {
		bg_note_till = current_time + note_duration;
		bg_note_till_pause = 0;
		nosound = 1;
		ET0 = 1;
		return;
	    }

	    if( pitch_idx >= 0 )
	    {
		note_period = pitch_table[pitch_idx];
		snd_set_note_period(note_period);

		// Dirty trick: enable sound processing
		// But calculate duration later
		ET0 = 1;
		bg_note_till += 1000; 

		nosound = 0;
	    }

	    switch( note->articulation ){
		case ART_NONE:     
		    pause_duration = (note_duration >> 2); 
		    note_duration  = (note_duration >> 2) + (note_duration >> 1); 
		    break;
		case ART_STACCATO: 
		    pause_duration = (note_duration >> 1);
		    note_duration  = (note_duration >> 1);
		    break;
		case ART_LEGATO: break;
	    }

	    bg_note_till       = current_time + note_duration;
	    bg_note_till_pause = current_time + pause_duration;
	    ET0 = 1;
	    break;
	}
	break;
    }
}

// void snd_play_note(struct note * note)
// {
//     uint16_t note_freq; 
//     int8_t pitch_idx;
//     unsigned long note_duration  = 0;
//     unsigned long pause_duration = 0;
//     unsigned long current_time;
// 
//     pitch_idx = note->pitch_idx;
//     note_duration = DURATION(note);
// 
//     if( pitch_idx == NOTE_REST )
//     {
// 	sleep(note_duration);
// 	return;
//     }
// 
//     if( pitch_idx >= 0 )
//     {
// 	note_freq = pitch_table[pitch_idx];
// 	ET0 = 1;
// 	snd_set_note_period(note_freq);
// 	current_time = get_time();
//     }
// 
//     switch( note->articulation ){
// 	case ART_LEGATO: break;
// 	case ART_NONE:     
// 	    pause_duration = (note_duration >> 2); 
// 	    note_duration  = (note_duration >> 2) + (note_duration >> 1); 
// 	    break;
// 	case ART_STACCATO: 
// 	    pause_duration = (note_duration >> 1);
// 	    note_duration  = (note_duration >> 1);
// 	    break;
//     }
// 
//     sleep_till(current_time + note_duration);
//     if( !bg_track )
// 	ET0 = 0;
//     sleep_till(current_time + pause_duration);
// }
// 
// void snd_play_track(struct track* track){
//     uint16_t i = 0;
//     struct note* note = &(track->notes[0]);
//     int8_t repeat;
// 
//     snd_set_bpm(track->bpm);
//     snd_set_time_signature(track->signature_upper, track->signature_lower);
//     repeat = track->repeat;
// 
// #define PLAY_TRACK() while((note+i)->pitch_idx != NOTE_TRACK_END) snd_play_note(note+(i++))
//     if( repeat == TRACK_INF_REPEAT ){
// 	while(1){
// 	    PLAY_TRACK();
// 	    i = 0;
// 	}
//     }
//     else do
//     {
// 	PLAY_TRACK();
// 	i = 0;
// 	--repeat;
//     } while (repeat >= 0);
// }

void snd_set_bg_track(struct track* track)
{
    bg_track = track;

    snd_set_bpm(track->bpm);
    snd_set_time_signature(track->signature_upper, track->signature_lower);

    bg_note_idx = 0;
    bg_note_till = bg_note_till_pause = 0;
    bg_repeat_count = track->repeat;
    nosound = 1;

    ET0 = 1;
}


void snd_init(void)
{
    recalculate_whole_note_duration();
}
