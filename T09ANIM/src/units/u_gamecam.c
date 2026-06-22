/* FILE NAME: control.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 13.06.2026
 */

#include "units.h"
#include "map/map.h"

typedef struct tagUNIT_GAMECAM me3UNIT_GAMECAM;
struct tagUNIT_GAMECAM
{
  me3UNIT_BASE_FIELDS;
  DBL Speed;
  DBL AngleSpeed;
};

static VOID ME3_UnitInit(me3UNIT_GAMECAM *Uni, me3ANIM *Ani)
{
  Uni->Speed = 7;
  Uni->AngleSpeed = 0.006;

  ME3_RndCamLoc = VecSet(2, 0, -5);
  ME3_RndCamDir = VecSet(-1, 0, 0);
  ME3_RndCamAt = VecSet(-1, 0, 0);
  ME3_RndCamUp = VecSet(0, 1, 0);
}

static VOID ME3_UnitResponse(me3UNIT_GAMECAM *Uni, me3ANIM *Ani)
{
  static DBL Angle = 0, AngleY = 0;
  VEC MoveDir;
  VEC PreCL;

  Angle += Ani->Keys[VK_LBUTTON] * Ani->Mdx * Uni->AngleSpeed;
  AngleY -= Ani->Keys[VK_LBUTTON] * Ani->Mdy * Uni->AngleSpeed;
  if (AngleY > 0.4)
    AngleY = 0.4;
  if (AngleY < -0.5)
    AngleY = -0.5;


  ME3_RndCamDir.X = cos(Angle) * cos(AngleY);
  ME3_RndCamDir.Z = sin(Angle) * cos(AngleY);
  ME3_RndCamDir.Y = sin(AngleY);

  MoveDir.X = cos(Angle);
  MoveDir.Z = sin(Angle);
  MoveDir.Y = 0;


  MoveDir = VecNormalize(MoveDir);

  ME3_RndCamRight = VecNormalize(VecCrossVec(ME3_RndCamDir, VecSet(0, 1, 0)));
 
  ME3_RndCamUp = VecSet(0, 1, 0);

  if (Ani->Keys['W'])
  {
    PreCL = VecAddVec(ME3_RndCamLoc, VecMulNum(MoveDir, Uni->Speed * Ani->GlobalDeltaTime));
    if (!ME3_CaminWall(PreCL.X, PreCL.Z))
      ME3_RndCamLoc = PreCL;
  }

  if (Ani->Keys['S'])
  {
    PreCL = VecSubVec(ME3_RndCamLoc, VecMulNum(MoveDir, Uni->Speed * Ani->GlobalDeltaTime));
    if (!ME3_CaminWall(PreCL.X, PreCL.Z))
      ME3_RndCamLoc = PreCL;
  }

  ME3_RndCamSet(ME3_RndCamLoc,
    VecAddVec(ME3_RndCamLoc, ME3_RndCamDir),
    ME3_RndCamUp
  );
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