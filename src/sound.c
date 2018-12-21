#include "aduc812.h"
#include "sound.h"
#include "system.h"
#include "timer.h"

#include <stdint.h>

#include "led.h" // DEBUG

/* 
 * Python script for frequencies:
 *
 * a0 = 27.5
 * for octave in range(1,8):
 *  print(', '.join(str(int(a0*2**(octave+n/12))) for n in range(0,11)))
 */

XDATA(uint16_t) note_table[72] = {
    55, 58, 61, 65, 69, 73, 77, 82, 87, 92, 97,
    110, 116, 123, 130, 138, 146, 155, 164, 174, 184, 195,
    220, 233, 246, 261, 277, 293, 311, 329, 349, 369, 391,
    440, 466, 493, 523, 554, 587, 622, 659, 698, 739, 783,
    880, 932, 987, 1046, 1108, 1174, 1244, 1318, 1396, 1479, 1567,
    1760, 1864, 1975, 2093, 2217, 2349, 2489, 2637, 2793, 2959, 3135,
    3520, 3729, 3951, 4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271,
};

uint8_t note_period[2] = { 0xff, 0xff };

void beep(void)
{
    xor_ena(0x1C);
}

void set_note_period(uint16_t note_freq)
{
    uint16_t delay = TIMER_FREQ / (2 * note_freq);
    uint8_t* ptr = (uint8_t*)&delay;
    // sdcc -> little endian? aduc -> big endian?
    note_period[0] = 0xFF - ptr[0];
    note_period[1] = 0xFF - ptr[1];
}

void play_note(struct note * note)
{
    uint16_t note_freq = note->freq;

    if( note_freq != 0 )
    {
	ET0 = 1;
	set_note_period(note_freq);
    }
    sleep(note->duration);
    ET0 = 0;
}
