#ifndef __rndres_h_
#define __rndres_h_

#include "anim/rnd/rnd.h"

VOID ME3_RndResInit( VOID );
VOID ME3_RndResClose( VOID );
VOID ME3_RndShdInit( VOID );
VOID ME3_RndShdClose( VOID );
VOID ME3_RndShdUpdate( VOID );

extern UINT ME3_RndProgId;

#endif /*__rndres_h_*/