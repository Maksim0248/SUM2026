/* FILE NAME: control.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 13.06.2026
 */

#include "units.h"

FLT a = 0;

typedef struct tagUNIT_CONTROL me3UNIT_CONTROL;
struct tagUNIT_CONTROL
{
  me3UNIT_BASE_FIELDS;
  DBL Speed;
  DBL AngleSpeed;
  //VEC CamAt;
  //VEC CamLoc;
};

static VOID ME3_UnitInit(me3UNIT_CONTROL *Uni, me3ANIM *Ani)
{
  Uni->Speed = 10;
  //Uni->CamAt = VecSet(0, 4, 0);
  //Uni->CamLoc = VecSet(-50, 5, 0);
  ME3_RndCamLoc = VecSet(-50, 5, 0);
  Uni->AngleSpeed = 0.008;
}

static VOID ME3_UnitResponse(me3UNIT_CONTROL *Uni, me3ANIM *Ani)
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

  FLT Dist, plen;
  DBL cosT, sinT, cosP, sinP, Azimuth, Elevator;
  Dist = VecLen(VecSubVec(ME3_RndCamAt, ME3_RndCamLoc));
 
  cosT = (ME3_RndCamLoc.Y - ME3_RndCamAt.Y) / Dist;
  sinT = sqrt(1 - cosT * cosT);
 
  plen = Dist * sinT;
  cosP = (ME3_RndCamLoc.Z - ME3_RndCamAt.Z) / plen;
  sinP = (ME3_RndCamLoc.X - ME3_RndCamAt.X) / plen;
 
  Azimuth = R2D(atan2(sinP, cosP));
  Elevator = R2D(atan2(sinT, cosT));   /*!!! acos(cosT)*/
 
  Azimuth += Ani->GlobalDeltaTime * 40 * (Ani->Keys[VK_RIGHT] - Ani->Keys[VK_LEFT]);
  Elevator += Ani->GlobalDeltaTime * 40 * (Ani->Keys[VK_DOWN] - Ani->Keys[VK_UP]);
  Dist += Ani->GlobalDeltaTime * 50 * (Ani->Mdz);
  if (Elevator < 0.1)
    Elevator = 0.1;
  if (Elevator > 178)
    Elevator = 178;
  if (Dist < 0.001)
    Dist = 0.001;
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  ME3_RndCamLoc =
    PointTransform(VecSet(0, Dist, 0),
      MatrMulMatr(MatrRotateX(Elevator),
                  MatrRotateY(Azimuth)));
  
  /*ME3_RndCamLoc =
  PointTransform(VecSet(0, Dist, 0),
    MatrRotateX(Elevator));*/
  
  ME3_RndCamSet(ME3_RndCamLoc, ME3_RndCamAt, VecSet(0, 1, 0));
}

static VOID ME3_UnitRender(me3UNIT_CONTROL *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitClose(me3UNIT_CONTROL *Uni, me3ANIM *Ani)
{
}

me3UNIT * ME3_UnitCreateCam( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_CONTROL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}