/* FILE NAME: u_COW.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include <time.h>
#include "units.h"

typedef struct tagUNIT_COW me3UNIT_G3DM;
struct tagUNIT_COW
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIMS Model;
};

static VOID ME3_UnitInit( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  Uni->Pos = VecSet(0, 3, 1);
  ME3_RndPrimsLoad(&Uni->Model, "bin/models/Minion2.g3dm");
  Uni->Model.Trans = MatrRotateY(0);
}

static VOID ME3_UnitResponse( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
}

static VOID ME3_UnitRender( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(5, 5, 5))), MatrRotateX(0)));
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