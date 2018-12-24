#ifndef __bindings_h__
#define __bindings_h__

/*
 * Note:
 * Some models/instances of SDK have malfunctioning keyboards:
 * First column of keys doesn't work at all
 * So provide alternative bindings using BROKEN_KEYBOARD macro
 */

#define BIND_OK_DESC "OK (A)"
#define BIND_OK      'A'
#define BIND_OK_S    "A"

#define BIND_CANCEL_DESC "Cancel (B)"
#define BIND_CANCEL      'B'
#define BIND_CANCEL_S    "B"



#ifndef BROKEN_KEYBOARD
#define BIND_BACKSPACE_DESC "Backspace (*)"
#define BIND_BACKSPACE      '*'
#define BIND_BACKSPACE_S    "*"
#else
#define BIND_BACKSPACE_DESC "Backspace (D)"
#define BIND_BACKSPACE      'D'
#define BIND_BACKSPACE_S    "D"
#endif /* BROKEN_KEYBOARD */

#define BIND_ENTER_DESC "Enter (#)"
#define BIND_ENTER      '#'
#define BIND_ENTER_S    "#"



#define BIND_FORWARD_DESC "Forward (2)"
#define BIND_FORWARD      '2'
#define BIND_FORWARD_S    "2"

#define BIND_BACK_DESC "Back (8)"
#define BIND_BACK      '8'
#define BIND_BACK_S    "8"

#ifndef BROKEN_KEYBOARD
#define BIND_LEFT_DESC "Left (4)"
#define BIND_LEFT      '4'
#define BIND_LEFT_S    "4"
#else
#define BIND_LEFT_DESC "Left (5)"
#define BIND_LEFT      '5'
#define BIND_LEFT_S    "5"
#endif /* BROKEN_KEYBOARD */

#define BIND_RIGHT_DESC "Right (6)"
#define BIND_RIGHT      '6'
#define BIND_RIGHT_S    "6"




#endif /* __bindings_h__ */
