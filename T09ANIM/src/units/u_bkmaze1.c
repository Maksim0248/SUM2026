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
  INT MtlNo;
};

static VOID ME3_UnitInit(me3UNIT_BKM *Uni, me3ANIM *Ani)
{
  me3MATERIAL mtl = ME3_RndMtlGetDef();
  
  Uni->Pos = VecSet(10, 2, 6);
  strcpy(mtl.Name, "BKMaze1");
  mtl.ShdNo = ME3_RndShdAdd("tex");  
  Uni->MtlNo = ME3_RndMtlAdd(&mtl); /*add to array of mtl and back number in array*/

  mtl.Tex[0] = ME3_RndTexAdd("bin\\textures\\BRwall.g24", 0, 0, 0, NULL);
  Uni->Pr.MtlNo = Uni->MtlNo;
  ME3_RndPrimLoad(&Uni->Pr, "bin/models/Maze.obj");
}

static VOID ME3_UnitResponse(me3UNIT_BKM *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitRender(me3UNIT_BKM *Uni, me3ANIM *Ani, me3PRIM *sph)
{
  ME3_RndPrimDraw(&Uni->Pr, MatrMulMatr(MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(1, 1, 1))), MatrRotateY(180)));
}

static VOID ME3_UnitClose(me3UNIT_BKM *Uni, me3ANIM *Ani)
{
  ME3_RndPrimFree(&Uni->Pr);
}


me3UNIT * ME3_UnitCreateBKMaze1( VOID )
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