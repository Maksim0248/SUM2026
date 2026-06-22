/* FILE NAME: u_COW.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include <time.h>
#include "units.h"

typedef struct tagUNIT_COW me3UNIT_COW;
struct tagUNIT_COW
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIM Cw;
};

static VOID ME3_UnitInit(me3UNIT_COW *Uni, me3ANIM *Ani)
{
  ME3_RndPrimLoad(&Uni->Cw, "bin/models/Maze.obj");
  //Uni->Pos = VecSet(0, 0.2, 0.3);
  Uni->Pos = VecSet(0, -1, 0);
}

static VOID ME3_UnitResponse(me3UNIT_COW *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitRender(me3UNIT_COW *Uni, me3ANIM *Ani, me3PRIM *sph)
{
  Uni->Cw.MtlNo = 0;
  ME3_RndPrimDraw(&Uni->Cw, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(3.4, 3.4, 3.4))), MatrRotateY(90)));
}

static VOID ME3_UnitClose(me3UNIT_COW *Uni, me3ANIM *Ani)
{
  ME3_RndPrimFree(&Uni->Cw);
}


me3UNIT * ME3_UnitCreateCow( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_COW))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}