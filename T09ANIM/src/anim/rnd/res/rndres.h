#ifndef __rndres_h_
#define __rndres_h_

#include "anim/rnd/rnd.h"


VOID ME3_RndResInit( VOID );
VOID ME3_RndResClose( VOID );
VOID ME3_RndShdInit( VOID );
VOID ME3_RndShdClose( VOID );
VOID ME3_RndShdUpdate( VOID );

#define ME3_MAX_SHADERS 30
#define ME3_STR_MAX 500

extern UINT ME3_RndProgId;
extern me3SHADER ME3_RndShaders[ME3_MAX_SHADERS];

typedef struct tagme3SHADER
{
  CHAR Name[ME3_STR_MAX];
  UINT ProgId;
} me3SHADER;

#endif /*__rndres_h_*/