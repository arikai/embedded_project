#!/usr/bin/env python3

import os.path as path
from sys import argv

filepath = argv[1] if len(argv) > 1 else './include/bindings.h'
broken_kb_macro = 'BROKEN_KEYBOARD'

bindings = (
# Name in BIND_NAME | Key | Alternative Key ( for broken keyboards )
# None is used for newline
        ( 'OK'        , 'A'          ),
        ( 'Cancel'    , 'B'          ),
        None, None,

        # Text input
        ( 'Backspace' , '*' , 'D'    ),
        ( 'Enter'     , '#'          ),
        None, None,

        # Overworld
        ( 'Forward'   , '2'          ),
        ( 'Back'      , '8'          ),
        ( 'Left'      , '4' , '5'    ),
        ( 'Right'     , '6'          ),
        None,

        # ( 'Discover'  , 'D'          ),
        None, 
)

with open(filepath, 'w+') as f:
    include_guard_name = '__{}__'.format(path.basename(filepath).replace('.', '_'));
    f.write('''\
#ifndef {}
#define {}

/*
 * Note:
 * Some models/instances of SDK have malfunctioning keyboards:
 * First column of keys doesn't work at all
 * So provide alternative bindings using BROKEN_KEYBOARD macro
 */

'''
    .format(include_guard_name, include_guard_name))

    def write_binding(name, key):
        macro = 'BIND_{}'.format(name.upper())
        f.write('#define {}_DESC "{} ({})"\n'.format( macro, name, key ))
        f.write('#define {}      \'{}\'\n'.format( macro, key ))
        f.write('#define {}_S    "{}"\n'.format( macro, key ))

    for binding in bindings:
        if binding == None: 
            f.write('\n');
            continue;

        if len(binding) == 2:
            write_binding(*binding);
            f.write('\n');

        elif len(binding) == 3:
            name, key, akey = binding
            f.write('#ifndef {}\n'.format(broken_kb_macro))
            write_binding(name, key)
            f.write('#else\n')
            write_binding(name, akey)
            f.write('#endif /* {} */\n\n'.format(broken_kb_macro))

    f.write('\n#endif /* {} */\n'.format(include_guard_name))
