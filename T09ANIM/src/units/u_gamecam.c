/* FILE NAME: control.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 13.06.2026
 */

#include "units.h"

typedef struct tagUNIT_GAMECAM me3UNIT_GAMECAM;
struct tagUNIT_GAMECAM
{
  me3UNIT_BASE_FIELDS;
  DBL Speed;
  DBL AngleSpeed;
};

static VOID ME3_UnitInit(me3UNIT_GAMECAM *Uni, me3ANIM *Ani)
{
  Uni->Speed = 10;
  ME3_RndCamLoc = VecSet(0, 0, 0);
  Uni->AngleSpeed = 0.008;
  ME3_RndCamAt = VecSet(1, 0, 0);
  ME3_RndCamUp = VecSet(0, 1, 0);
}

static VOID ME3_UnitResponse(me3UNIT_GAMECAM *Uni, me3ANIM *Ani)
{
  /*DBL SpeedX;
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
    a += 0.1;
  if (Ani->Keys[VK_LEFT] == 1)
    a += -0.1;*/
  //Uni->PosCam.X += SpeedX * Ani->DeltaTime;
  //ME3_RndCamSet(VecAddVec(VecSet(0, 0, 0), Uni->PosCam), VecAddVec(VecSet(cos(a), 0, sin(a)), Uni->PosCam), VecSet(0, 1, 0));
  /*
  VEC d;
  d = VecNormalize(VecSubVec(Uni->CamAt, Uni->CamLoc));
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
    VecMulNum(d, Ani->GlobalDeltaTime * (Uni->Speed + Ani->Keys[VK_SHIFT] * 30) *
    (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]) + Ani->Mdz / 50));
  Uni->CamLoc =
    PointTransform(Uni->CamLoc,
    MatrRotateY(Ani->Keys[VK_LBUTTON] * Uni->AngleSpeed * Ani->Mdx));
  ME3_RndCamSet(Uni->CamLoc, Uni->CamAt, VecSet(0, 1, 0));
  */
  FLT Dist;

  Dist = VecLen(VecSubVec(ME3_RndCamAt, ME3_RndCamLoc));
  Dist += Ani->GlobalDeltaTime * 50 * (Ani->Keys['W']);
  Dist -= Ani->GlobalDeltaTime * 50 * (Ani->Keys['S']);

  //ME3_RndCamLoc = PointTransform(VecSet(0, Dist, 0), abs(sin(Ani->GlobalTime)),);
 
  //ME3_RndCamSet(ME3_RndCamLoc, VecSet(1, 0, 0), VecSet(0, 1, 0));
}

static VOID ME3_UnitRender(me3UNIT_GAMECAM *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitClose(me3UNIT_GAMECAM *Uni, me3ANIM *Ani)
{
}

me3UNIT * ME3_UnitCreateGameCam( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_GAMECAM))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}