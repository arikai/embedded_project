#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "common.h"
#include "system.h"

extern uint8_t keys[16];

enum kb_event_type { KB_NONE = 0, KB_PRESS, KB_RELEASE };

struct kb_event {
    uint8_t key;
    enum kb_event_type type;
};

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
 * because SDCC sucks and doesn't support struct arguments to functions
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
