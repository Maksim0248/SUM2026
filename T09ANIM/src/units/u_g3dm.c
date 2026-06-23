/* FILE NAME: u_COW.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include <time.h>
#include "units.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm")


static DBL t = 0;
static DBL k = 0;
INT Sndpl = 0;

typedef struct tagUNIT_COW me3UNIT_G3DM;
struct tagUNIT_COW
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIMS Model;
};

static VOID ME3_UnitInit( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  Uni->Pos = VecSet(-80, -120, -100);
  ME3_RndPrimsLoad(&Uni->Model, "bin/models/ghost.g3dm");
  Uni->Model.Trans = MatrRotateY(270);
  mciSendString("open bin/snd/run.mp3 type mpegvideo alias rn", NULL, 0, NULL);
  mciSendString("open bin/snd/JUMPSCARE.mp3 type mpegvideo alias abc", NULL, 0, NULL);
  mciSendString("open bin/snd/miniJS.mp3 type mpegvideo alias minijs", NULL, 0, NULL);
  mciSendString("open bin/snd/scr3.mp3 type mpegvideo alias s3", NULL, 0, NULL);
  /*for (i = 0; i < Uni->Model.NumOfPrims; i++)
    Uni->Model.Prims[i].MtlNo = 0;*/
}

static VOID ME3_UnitResponse( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  if (ME3_RndCamLoc.X < 3.4 && ME3_RndCamLoc.X > -3.4 && ME3_RndCamLoc.Z < 0 && ME3_RndCamLoc.Z > -3.5)
  {
    mciSendString("open bin/snd/JUMPSCARE.mp3 type mpegvideo alias abc", NULL, 0, NULL);
    mciSendString("play abc", NULL, 0, NULL);
    ME3_ScrState = 1;
    ME3_RndCamSet(VecSet(-4, 0, -38), VecSet(10, -2, -38), VecSet(0, 1, 0));
  }

}

static VOID ME3_UnitRender( me3UNIT_G3DM *Uni, me3ANIM *Ani )
{
  static INT MJ = 0, M3 = 0;

  if (ME3_RndCamLoc.Z > -5 && ME3_RndCamLoc.Z < 1)
  {
    ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(0.02, 0.02, 0.02))), MatrRotateX(sin(Ani->GlobalTime) * 10)));
    if (!MJ && ME3_RndCamLoc.Z > -3.5 && ME3_RndCamLoc.Z < 0)
    {
      mciSendString("play minijs", NULL, 0, NULL);
      MJ = 1;
    }
  }
  if (ME3_RndCamLoc.Z < 3.5 && ME3_RndCamLoc.X < -6.8)
  {
    VEC pos1, posmin, posmax, pos2;

    
    t += 140 * Ani->DeltaTime;
    pos1 = VecAddVec(VecSet(-430, -120, 530), VecSet(0, 0, -t));
    pos2 = VecMulNum(VecAddVec(VecSet(-430, -120, 530), VecSet(0, 0, -t)), 0.02);
    posmin = VecSet(pos2.X - 3, pos2.Y, pos2.Z - 3);
    posmax = VecSet(pos2.X + 3, pos2.Y, pos2.Z + 3);
    ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrRotateY(270), MatrMulMatr(MatrTranslate(pos1), MatrScale(VecSet(0.02, 0.02, 0.02)))));
    
    if (!Sndpl)
    {
      mciSendString("play rn", NULL, 0, NULL);
      Sndpl = 1;
    }
    if (ME3_RndCamLoc.X > posmin.X && ME3_RndCamLoc.Z > posmin.Z && ME3_RndCamLoc.X < posmax.X && ME3_RndCamLoc.Z < posmax.Z)
    {
      mciSendString("play abc", NULL, 0, NULL);
      //mciSendString("resume abc", NULL, 0, NULL);
 
      ME3_ScrState = 1;
      ME3_RndCamSet(VecSet(-4, 0, -38), VecSet(10, -2, -38), VecSet(0, 1, 0));
    }
  }
  if ((ME3_RndCamLoc.X > 3.4 && ME3_RndCamLoc.X < 10) && (ME3_RndCamLoc.Z < 9.7 && ME3_RndCamLoc.Z > 7))
  {

    k += 5 * Ani->DeltaTime;
    ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrRotateY(90), MatrMulMatr(MatrScale(VecSet(0.02, 0.02, 0.02)), MatrTranslate(VecAddVec(VecSet(3.8, -2.5, 5.5), VecSet(k, 0, 0))))));
    if (!M3)
    {
      mciSendString("play s3", NULL, 0, NULL);
      M3 = 1;
    }
  }

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