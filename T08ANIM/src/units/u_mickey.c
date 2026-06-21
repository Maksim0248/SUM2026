/* FILE NAME: u_mickey.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include "units.h"

typedef struct tagUNIT_MICKEY me3UNIT_MICKEY;
struct tagUNIT_MICKEY
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIM Bl;
};

static VOID ME3_UnitInit(me3UNIT_MICKEY *Uni, me3ANIM *Ani)
{
  ME3_RndPrimLoad(&Uni->Bl, "bin/models/cow.obj");
  Uni->Pos = VecSet(0, 0, 0);
}

static VOID ME3_UnitResponse(me3UNIT_MICKEY *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitRender(me3UNIT_MICKEY *Uni, me3ANIM *Ani)
{
  ME3_RndPrimDraw(&Uni->Bl, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(1.2, 1.2, 1.2))), MatrRotateZ(cos(10 * Ani->GlobalTime))));
}

static VOID ME3_UnitClose(me3UNIT_MICKEY *Uni, me3ANIM *Ani)
{
  ME3_RndPrimFree(&Uni->Bl);
}


me3UNIT * ME3_UnitCreateMickey( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_MICKEY))) == NULL)/*create empty struct*/
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}