#include <time.h>
#include "units.h"


typedef struct tagUNIT_SCR me3UNIT_SCR;
struct tagUNIT_SCR
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIMS Model;
  me3PRIMS Conf;
  me3PRIMS Conf2;
};

static VOID ME3_UnitInit( me3UNIT_SCR *Uni, me3ANIM *Ani )
{
  INT i;
  me3MATERIAL mtl = ME3_RndMtlGetDef(), mtl2 = ME3_RndMtlGetDef(), mtl3 = ME3_RndMtlGetDef(), mtl4 = ME3_RndMtlGetDef();

  strcpy(mtl.Name, "Confetti1");
  mtl.Kd = VecSet(1, 0, 0);
  mtl.Ka = VecSet1(0.5);
  mtl.Ks = VecSet1(0.9);
  mtl.Ph = 0.7;
  mtl.ShdNo = 0;

  strcpy(mtl2.Name, "Confetti2");
  mtl2.Kd = VecSet(0, 0, 1);
  mtl2.Ka = VecSet1(0.5);
  mtl2.Ks = VecSet1(0.9);
  mtl2.Ph = 0.7;
  mtl2.ShdNo = 0;

  strcpy(mtl3.Name, "Confetti3");
  mtl3.Kd = VecSet(0, 1, 0);
  mtl3.Ka = VecSet1(0.5);
  mtl3.Ks = VecSet1(0.9);
  mtl3.Ph = 0.7;
  mtl3.ShdNo = 0;

  strcpy(mtl4.Name, "Confetti4");
  mtl4.Kd = VecSet(1, 1, 0);
  mtl4.Ka = VecSet1(0.5);
  mtl4.Ks = VecSet1(0.9);
  mtl4.Ph = 0.7;
  mtl4.ShdNo = 0;

  Uni->Pos = VecSet(-40, -120, -1900);
  ME3_RndPrimsLoad(&Uni->Model, "bin/models/ghost.g3dm");
  ME3_RndPrimsLoad(&Uni->Conf, "bin/models/Conf.g3dm");
  ME3_RndPrimsLoad(&Uni->Conf2, "bin/models/Conf.g3dm");

  for (i = 0; i < Uni->Conf.NumOfPrims; i++)
  {
    if (i % 4 == 0)
      Uni->Conf.Prims[i].MtlNo = ME3_RndMtlAdd(&mtl);
    if (i % 4 == 1)
      Uni->Conf.Prims[i].MtlNo = ME3_RndMtlAdd(&mtl2);
    if (i % 4 == 2)
      Uni->Conf.Prims[i].MtlNo = ME3_RndMtlAdd(&mtl3);
    if (i % 4 == 3)
      Uni->Conf.Prims[i].MtlNo = ME3_RndMtlAdd(&mtl4);

  }

  Uni->Model.Trans = MatrRotateY(270);
  /*for (i = 0; i < Uni->Model.NumOfPrims; i++)
    Uni->Model.Prims[i].MtlNo = 0;*/
}

static VOID ME3_UnitResponse( me3UNIT_SCR *Uni, me3ANIM *Ani )
{
}

static VOID ME3_UnitRender( me3UNIT_SCR *Uni, me3ANIM *Ani )
{
  MATR Rot;
  if (ME3_Win)
  {
    ME3_RndPrimsDraw(&Uni->Conf, MatrMulMatr(MatrMulMatr(MatrRotateX(sin(Ani->GlobalTime * 5) * 10), MatrRotateY(sin(Ani->GlobalTime * 5) * 10)), MatrMulMatr(MatrTranslate(VecSet(9, -1, -49)), MatrScale(VecSet(1, 1, 1)))));
    ME3_RndPrimsDraw(&Uni->Conf, MatrMulMatr(MatrMulMatr(MatrRotateX(sin(Ani->GlobalTime * 5) * 10), MatrRotateY(-sin(Ani->GlobalTime * 5) * 10)), MatrMulMatr(MatrTranslate(VecSet(9, -1, -43)), MatrScale(VecSet(1, 1, 1)))));
    
    Rot = MatrMulMatr(MatrMulMatr(MatrRotateX(sin(Ani->GlobalTime * 3)), MatrRotateZ(sin(Ani->GlobalTime * 30))), MatrRotateY(sin(Ani->GlobalTime * 3)));
  }
  else
    Rot = MatrRotateZ(abs(sin(Ani->GlobalTime * 20) * 5));
  ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(0.02, 0.02, 0.02))), Rot));
}

static VOID ME3_UnitClose( me3UNIT_SCR *Uni, me3ANIM *Ani )
{
  ME3_RndPrimsFree(&Uni->Model);
  ME3_RndPrimsFree(&Uni->Conf);
}


me3UNIT * ME3_UnitCreateScr( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_SCR))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}