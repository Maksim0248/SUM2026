/* FILE NAME: u_COW.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include <time.h>
#include "units.h"

BOOL k = TRUE;

typedef struct tagUNIT_COW me3UNIT_G3DM;
struct tagUNIT_COW
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIMS Model;
};

static VOID ME3_UnitInit( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  Uni->Pos = ME3_RndCamLoc;
  ME3_RndPrimsLoad(&Uni->Model, "bin/models/flashlight.g3dm");
}

static VOID ME3_UnitResponse( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  if (Ani->KeysClick['E'])
    k = !k;
  if (ME3_ScrState)
    k = 0;
}

static VOID ME3_UnitRender( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  MATR m = MatrMulMatr(MatrScale(VecSet(0.0008, 0.0008, 0.0008)), MatrTranslate(VecAddVec(ME3_RndCamLoc, VecSet(0, -0.6, 0))));

  FLT Angle = R2D(atan2(ME3_RndCamDir.Z, ME3_RndCamDir.X));

  if (k)
    ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrRotateY(270 - Angle), m));
}

static VOID ME3_UnitClose( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  ME3_RndPrimsFree(&Uni->Model);
}


me3UNIT * ME3_UnitCreateFL( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_G3DM))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}