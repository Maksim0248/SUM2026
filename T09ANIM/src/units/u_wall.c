/* FILE NAME: u_bkmaze1.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 18.06.2026
 */

#include <time.h>
#include <stdio.h>
#include "units.h"

typedef struct tagUNIT_BKM me3UNIT_BKM;
struct tagUNIT_BKM
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIM Pr;
};

static VOID ME3_UnitInit(me3UNIT_BKM *Uni, me3ANIM *Ani)
{
  me3MATERIAL mtl = ME3_RndMtlGetDef();
  me3VERTEX V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{20, 0, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 20, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}},
    {{20, 20, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}},
  };

  Uni->Pos = VecSet(10, 2, 6);
  strcpy(mtl.Name, "BKMaze1");
  mtl.ShdNo = ME3_RndShdAdd("tex");
  mtl.Tex[2] = ME3_RndTexAddFromFile("bin/textures/BRwall.g24");

  ME3_RndPrimCreate(&Uni->Pr, ME3_RND_PRIM_TRISTRIP, V, 4, NULL, 0);
  Uni->Pr.MtlNo = ME3_RndMtlAdd(&mtl); /*add to array of mtl and back number in array*/
}

static VOID ME3_UnitResponse(me3UNIT_BKM *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitRender(me3UNIT_BKM *Uni, me3ANIM *Ani)
{
  ME3_RndPrimDraw(&Uni->Pr, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(1, 1, 1))), MatrRotateY(90)));
}

static VOID ME3_UnitClose(me3UNIT_BKM *Uni, me3ANIM *Ani)
{
  ME3_RndPrimFree(&Uni->Pr);
}


me3UNIT * ME3_UnitCreateWall( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_BKM))) == NULL)
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}