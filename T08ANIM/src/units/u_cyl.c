/* FILE NAME: u_cyl.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include "units.h"

typedef struct tagUNIT_BALL me3UNIT_BALL;
struct tagUNIT_BALL
{
  me3UNIT_BASE_FIELDS;
  VEC Pos;
  me3PRIM Bl;
};

static VOID ME3_UnitInit(me3UNIT_BALL *Uni, me3ANIM *Ani)
{
  ME3_RndPrimCreateCyl(&Uni->Bl, 3, 20, 20, 10);
  Uni->Pos = VecSet(-0.5, 0, 2);
}

static VOID ME3_UnitResponse(me3UNIT_BALL *Uni, me3ANIM *Ani)
{
}

static VOID ME3_UnitRender(me3UNIT_BALL *Uni, me3ANIM *Ani, me3PRIM *sph)
{
  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCPenColor(Ani->hDC, RGB(20, 255, 60));

  ME3_RndPrimDraw(&Uni->Bl, MatrTranslate(Uni->Pos));
  SetDCPenColor(Ani->hDC, RGB(0, 0, 0));

}

static VOID ME3_UnitClose(me3UNIT_BALL *Uni, me3ANIM *Ani)
{
  ME3_RndPrimFree(&Uni->Bl);
}


me3UNIT * ME3_UnitCreateCyl( VOID )
{
  me3UNIT *Uni;
 
  if ((Uni = ME3_AnimUnitCreate(sizeof(me3UNIT_BALL))) == NULL)/*create empty struct*/
    return NULL;

  Uni->Init = (VOID *)ME3_UnitInit;
  Uni->Close = (VOID *)ME3_UnitClose;
  Uni->Render = (VOID *)ME3_UnitRender;
  Uni->Response = (VOID *)ME3_UnitResponse;

  return Uni;
}