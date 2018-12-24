#ifndef __TRACKS_H__
#define __TRACKS_H__

#include "common.h"
#define TRACK(name) XDATA(struct track) name

extern TRACK(  ut_theme_track );
extern TRACK(     intro_track );
extern TRACK( overworld_track );
extern TRACK(    battle_track );

#endif /* __TRACKS_H__ */
