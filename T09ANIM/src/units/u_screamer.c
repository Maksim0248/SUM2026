#include <time.h>
#include "units.h"


typedef struct tagUNIT_SCR me3UNIT_SCR;
struct tagUNIT_SCR
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIMS Model;
};

static VOID ME3_UnitInit( me3UNIT_SCR *Uni, me3ANIM *Ani )
{
  INT i;
  Uni->Pos = VecSet(-40, -120, -1900);
  ME3_RndPrimsLoad(&Uni->Model, "bin/models/ghost.g3dm");
  Uni->Model.Trans = MatrRotateY(270);
  /*for (i = 0; i < Uni->Model.NumOfPrims; i++)
    Uni->Model.Prims[i].MtlNo = 0;*/
}

static VOID ME3_UnitResponse( me3UNIT_SCR *Uni, me3ANIM *Ani )
{
}

static VOID ME3_UnitRender( me3UNIT_SCR *Uni, me3ANIM *Ani )
{
  ME3_RndPrimsDraw(&Uni->Model, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(0.02, 0.02, 0.02))), MatrRotateZ(abs(sin(Ani->GlobalTime * 20) * 5))));
}

static VOID ME3_UnitClose( me3UNIT_SCR *Uni, me3ANIM *Ani )
{
  ME3_RndPrimsFree(&Uni->Model);
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