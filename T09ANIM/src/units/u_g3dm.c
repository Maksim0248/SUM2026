/* FILE NAME: u_COW.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include <time.h>
#include "units.h"

DBL t = 0;

typedef struct tagUNIT_COW me3UNIT_G3DM;
struct tagUNIT_COW
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIMS Model;
};

static VOID ME3_UnitInit( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  INT i;
  Uni->Pos = VecSet(-80, -120, -100);
  ME3_RndPrimsLoad(&Uni->Model, "bin/models/ghost.g3dm");
  Uni->Model.Trans = MatrRotateY(270);
  /*for (i = 0; i < Uni->Model.NumOfPrims; i++)
    Uni->Model.Prims[i].MtlNo = 0;*/
}

static VOID ME3_UnitResponse( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
}

static VOID ME3_UnitRender( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  DBL D;

  if (ME3_RndCamLoc.Z > -3.7 && ME3_RndCamLoc.Z < 0)
    ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(0.02, 0.02, 0.02))), MatrRotateX(sin(Ani->GlobalTime) * 10)));
  if (ME3_RndCamLoc.X < 3.4 && ME3_RndCamLoc.X > -3.4 && ME3_RndCamLoc.Z < 0 && ME3_RndCamLoc.Z > -3.5)
  {
    ME3_ScrState = 1;
    ME3_RndCamSet(VecSet(-4, 0, -38), VecSet(-10, 0, -40), VecSet(0, 1, 0));
  }
  if (ME3_RndCamLoc.Z < 3.5 && ME3_RndCamLoc.X < -6.8)
  {
    t += 60 * Ani->DeltaTime;
    ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrRotateY(270), MatrMulMatr(MatrTranslate(VecAddVec(VecSet(-420, -120, 420), VecSet(0, 0, -t))), MatrScale(VecSet(0.02, 0.02, 0.02)))));
  }
}

static VOID ME3_UnitClose( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  ME3_RndPrimsFree(&Uni->Model);
}


me3UNIT * ME3_UnitCreateG3dm( VOID )
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