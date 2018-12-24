#include "tracks.h"
#include "sound.h"

// TRACK( ut_theme_track ) = {
//     90,       // BPM
//     4, 4,     // Track signature
// 
//     TRACK_INF_REPEAT, // Repeat 1 times (play 2 times)
//     {
// 	N(B4 + 0, 8)
// 	N(B4 + 6, 8)
// 	N(B4 + 4, 8)
// 	N(B4 + 0, 8)
// 	N(B4 + 3, 8d)
// 	N(B4 + 3, 8d)
// 	N(B4 + 4, 4)
// 
// 	N(B4 + 0, 8)
// 	N(B4 + 4, 8)
// 	N(B4 + 0, 8)
// 	N(B4 + 3, 8d)
// 	N(B4 + 3, 8d)
// 	N(B4 + 4, 8)
// 
// 	N(B4 + 0, 8)
// 	N(B4 + 6, 8)
// 	N(B4 + 4, 8)
// 	N(B4 + 0, 8)
// 	N(B4 + 3, 8d)
// 	N(B4 + 3, 8d)
// 	N(B4 + 4, 4)
// 
// 	N(B4 + 0, 8)
// 	N(B4 + 4, 8)
// 	N(B4 + 8, 8)
// 	N(B4 + 6, 8d)
// 	N(B4 + 4, 8d)
// 	N(B4 + 6, 8)
// 
// 	END
//     }
// };

#define K2 (G4+1-24)
#define K3 (G4+1-12)
TRACK( intro_track ) = {
    60,
    4, 4,
    TRACK_NO_REPEAT,
    {
	S( K2, 1 )
	S( K2, 1 )
	S( K2, 1 )
	S( K2, 1 )
	S( K2, 1 )
	S( K2, 1 )
	S( K2, 1 )
	S( K2, 1 )

	R( 1 )
	R( 1 )
	
	L( K3+0, 4 )
	L( K3+1, 4 )
	L( K3+2, 4 )
	L( K3+3, 4 )
	L( K3+4, 4 )
	L( K3+5, 4 )
	L( K3+6, 4 )
	L( K3+7, 4 )

	END
    }
};
