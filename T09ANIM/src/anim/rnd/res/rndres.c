
#include "def.h"
#include "anim/rnd/rnd.h"

VOID ME3_RndResInit( VOID )
{
  ME3_RndShdInit();
  ME3_RndTexInit();
  ME3_RndMtlInit();
}

VOID ME3_RndResClose( VOID )
{
  ME3_RndMtlClose();
  ME3_RndShdClose();
  ME3_RndTexClose();
  
}
