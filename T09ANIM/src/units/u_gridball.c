/* FILE NAME: u_gridball.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 16.06.2026
 */

#include "units.h"

typedef struct tagUNIT_BALL me3UNIT_BALL;
struct tagUNIT_BALL
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIM Bl;
  me3GRID GrBl;
};

static VOID ME3_UnitInit( me3UNIT_BALL *Uni, me3ANIM *Ani )
{
  ME3_RndGridCreateSphere(&Uni->GrBl, 2, 10, 10);
  ME3_RndPrimFromGrid(&Uni->Bl, &Uni->GrBl);
  Uni->Pos = VecSet(0, 2, 0);
  ME3_RndGridFree(&Uni->GrBl);
}

static VOID ME3_UnitResponse( me3UNIT_BALL *Uni, me3ANIM *Ani )
{
}

static VOID ME3_UnitRender( me3UNIT_BALL *Uni, me3ANIM *Ani )
{
  Uni->Bl.MtlNo = 0;
  ME3_RndPrimDraw(&Uni->Bl, MatrTranslate(Uni->Pos));
}

static VOID ME3_UnitClose( me3UNIT_BALL *Uni, me3ANIM *Ani )
{
  ME3_RndPrimFree(&Uni->Bl);
}

me3UNIT * ME3_UnitCreateGridBall( VOID )
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