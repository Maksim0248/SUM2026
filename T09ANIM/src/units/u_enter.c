
#include <time.h>
#include "units.h"

typedef struct tagUNIT_DOOR me3UNIT_DOOR;
struct tagUNIT_DOOR
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIMS Model;
};

static VOID ME3_UnitInit( me3UNIT_DOOR *Uni, me3ANIM *Ani )
{
  Uni->Pos = ME3_RndCamLoc;
  ME3_RndPrimsLoad(&Uni->Model, "bin/models/door.g3dm");
  mciSendString("open bin/snd/win.mp3 type mpegvideo alias wn", NULL, 0, NULL);
}

static VOID ME3_UnitResponse( me3UNIT_DOOR *Uni, me3ANIM *Ani )
{
  static INT Mpl = 0;
  if (ME3_RndCamLoc.X > 9.9 && ME3_RndCamLoc.Z > -3.5 && ME3_RndCamLoc.Z < 0 && !Mpl)
  {
    Mpl = 1;
    mciSendString("play wn", NULL, 0, NULL);
    mciSendString("close bm", NULL, 0, NULL);
    ClearColor = VecSet4(0, 1, 0, 1);
    ME3_ScrState = 1;
    ME3_Win = 1;
    ME3_RndCamSet(VecSet(-4, 0, -38), VecSet(10, -2, -38), VecSet(0, 1, 0));
  }
}

static VOID ME3_UnitRender( me3UNIT_DOOR *Uni, me3ANIM *Ani )
{
  MATR m = MatrMulMatr(MatrScale(VecSet(2, 1.4, 2.9)), MatrTranslate(VecSet(10, -1.4, -1.7)));
  
  ME3_RndPrimsDraw(&Uni->Model, m);
}

static VOID ME3_UnitClose( me3UNIT_DOOR *Uni, me3ANIM *Ani )
{
  ME3_RndPrimsFree(&Uni->Model);
}


me3UNIT * ME3_UnitCreateEnter( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_DOOR))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}