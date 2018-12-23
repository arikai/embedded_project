#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "common.h"
#include "system.h"

#define KEYNUM 16
extern uint8_t keys[KEYNUM];

enum kb_event_type { 
    KB_NONE = 0, 
    KB_PRESS = 1, 
    KB_HOLD = 2, 
    KB_RELEASE = 4,

    // Masks
    KB_IS_PRESSED = 3 // If key was either just pressed or is held
};

struct kb_event {
    uint8_t key;
    uint8_t type; // avoid enum def since I do not trust SDCC
};

#define KB_SWITCH(ev) switch( ((uint16_t)ev.key) << 8 | ev.type )
#define KB_CASE(key, event) case (((uint16_t)key) << 8 | event )
#define KB_CASE2(key, event1, event2)  case (((uint16_t)key) << 8 | event1 ): \
				       /* FALLTHRU */                        \
			               case (((uint16_t)key) << 8 | event2 )
#define KB_IS(ev, mask) (ev.type & mask)

#define kb_raw() (read_max(KB))
#define kb_any_key_pressed() (kb_raw() ^ 0xf0) // VALID ONLY for zeroed 3-0 bits of KB

#define kb_set_mask(mask) write_max(KB, mask)

/*
 * Initialize keyboard and related registers
 */
void kb_init(void);

extern struct kb_event kb_event;

/*
 * Push new keyboard event to Event Queue
 * Must not be of KB_NONE type
 * NOTE: event's key and type passed separately
 * because SDCC lacks features and doesn't support struct arguments to functions
 */
void kb_push_event(uint8_t key, enum kb_event_type);

/*
 * Fill kb_event with first event in Event Queue
 * Return it's type
 * NOTE: YOU guess why it doesn't return struct.
 */
enum kb_event_type kb_poll_event(void);

void kb_process(void);

#endif /* __KEYBOARD_H__ */
