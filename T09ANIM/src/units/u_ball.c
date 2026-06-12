/* FILE NAME: u_ball.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include <time.h>

#include "units.h"

typedef struct tagUNIT_BALL me3UNIT_BALL;
struct tagUNIT_BALL
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIM Bl;
};

static VOID ME3_UnitInit(me3UNIT_BALL *Uni, me3ANIM *Ani)
{
  ME3_RndPrimCreateSphere(&Uni->Bl, 1, 10, 20);
  Uni->Pos = VecSet(0, 0.2, 0.3);
  Uni->Pos = VecSet(0, 0, 0);
}

static VOID ME3_UnitResponse(me3UNIT_BALL *Uni, me3ANIM *Ani)
{
  Uni->Pos.Y = fabs(sin(3 * clock() / 1000.0));
}

static VOID ME3_UnitRender(me3UNIT_BALL *Uni, me3ANIM *Ani, me3PRIM *sph)
{
  ME3_RndPrimDraw(&Uni->Bl, MatrTranslate(Uni->Pos));
}

static VOID ME3_UnitClose(me3UNIT_BALL *Uni, me3ANIM *Ani)
{
  ME3_RndPrimFree(&Uni->Bl);
}


me3UNIT * ME3_UnitCreateBall( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_BALL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}