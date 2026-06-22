#include "units.h"
#include <stdio.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm")

typedef struct tagUNIT_MUS me3UNIT_MUS;
struct tagUNIT_MUS
{
  me3UNIT_BASE_FIELDS;

};

static VOID ME3_UnitInit(me3UNIT_MUS *Uni, me3ANIM *Ani)
{
  mciSendString("open bin/snd/brmus.mp3 type mpegvideo alias bm", NULL, 0, NULL);
  mciSendString("play bm", NULL, 0, NULL);
 
 
}

static VOID ME3_UnitResponse(me3UNIT_MUS *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitRender(me3UNIT_MUS *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitClose(me3UNIT_MUS *Uni, me3ANIM *Ani)
{
  mciSendString("close bm", NULL, 0, NULL);
}

me3UNIT * ME3_UnitCreateMus( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_MUS))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}