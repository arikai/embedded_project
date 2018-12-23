#ifndef __DEMO_H__
#define __DEMO_H__

/*
 * Collection of demos, mostly used for testing purposes
 */
void piano_keyboard(void);

/*
 * Read keyboard and play notes indefinitely
 * Leds show current octave (on change) and played note
 * LCD Display shows played note
 * 2 3 A 5 6 B 8 9 - C major notes
 * 0 - set 4 octave, #/D - transpose -1/+1 octave
 */
void sound_test(void);

/*
 * Read input from keyboard and display on LCD
 */
void read_and_display_input(void);

#endif /* __DEMO_H__ */
