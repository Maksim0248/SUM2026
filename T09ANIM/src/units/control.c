/* FILE NAME: control.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 13.06.2026
 */

#include "units.h"

FLT a = 0;

typedef struct tagUNIT_CAM me3UNIT_CAM;
struct tagUNIT_CAM
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
};

static VOID ME3_UnitInit(me3UNIT_CAM *Uni, me3ANIM *Ani)
{
  Uni->Pos = VecSet(-50, 5, 0);
}

static VOID ME3_UnitResponse(me3UNIT_CAM *Uni, me3ANIM *Ani)
{
  DBL SpeedX;
  SpeedX = 0;

  if (Ani->Keys[VK_DOWN] == 1)
    SpeedX = -15;
  if ((Ani->Keys[VK_DOWN] == 1) && (Ani->Keys[VK_SHIFT] == 1))
    SpeedX = -50;
  if (Ani->Keys[VK_UP] == 1)
    SpeedX = 15;
  if ((Ani->Keys[VK_UP] == 1) && (Ani->Keys[VK_SHIFT] == 1))
    SpeedX = 50;
  if (Ani->Keys[VK_RIGHT] == 1)
    a += 0.01;
  if (Ani->Keys[VK_LEFT] == 1)
    a += -0.01;

  Uni->Pos.X += SpeedX * Ani->DeltaTime;
  ME3_RndCamSet(VecAddVec(VecSet(0, 0, 0), Uni->Pos), VecAddVec(VecSet(cos(a), 0, sin(a)), Uni->Pos), VecSet(0, 1, 0));
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