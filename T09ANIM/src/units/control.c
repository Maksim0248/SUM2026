/* FILE NAME: control.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 13.06.2026
 */

#include "units.h"

typedef struct tagUNIT_CAM me3UNIT_CAM;
struct tagUNIT_CAM
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
};

static VOID ME3_UnitInit(me3UNIT_CAM *Uni, me3ANIM *Ani)
{
  Uni->Pos = VecSet(5, 0, 0);
}

static VOID ME3_UnitResponse(me3UNIT_CAM *Uni, me3ANIM *Ani)
{
  DBL SpeedX, SpeedY;
  SpeedX = SpeedY = 0;

  if (Ani->Keys[VK_RIGHT] == 1)
    SpeedX = 2;
  if (Ani->Keys[VK_LEFT] == 1)
    SpeedX = -2;
  if (Ani->Keys[VK_DOWN] == 1)
    SpeedY = -2;
  if (Ani->Keys[VK_UP] == 1)
    SpeedY = 2;
  Uni->Pos.X += SpeedX * Ani->DeltaTime;
  Uni->Pos.Y += SpeedY * Ani->DeltaTime;
  ME3_RndCamSet(Uni->Pos, VecSet(0, 0, 0), VecSet(0, 1, 0));
}

static VOID ME3_UnitRender(me3UNIT_CAM *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitClose(me3UNIT_CAM *Uni, me3ANIM *Ani)
{
}

me3UNIT * ME3_UnitCreateCam( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_CAM))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}