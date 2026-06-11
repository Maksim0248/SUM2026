/* FILE NAME: anim.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */


#include "anim.h"
VOID ME3_AnimInit(HWND hWnd)
{
  memset(&ME3_Anim, 0, sizeof(vg4ANIM);
  ME3_Anim.hWnd = hWnd;
  ME3_RndInit(hWnd);
  ME3_Anim.hDC = ME3_hRndDCFrame;
}
/*ME3_AnimClose*/

VOID ME3_AnimResize( INT W, INT H )
{
  ME3_RndResize(W, H);
  ME3_Anim.H = H;
  ME3_Anim.W = W;
  ME3_AnimRender();
}

VOID ME3_AnimRender( VOID )
{
  INT i;

  for (i = 0; i < Anim.NumOfUnits; i++)
    ME3_Anim.Units[i]->Response(ME3_Anim.Units[i], &ME3_Anim);

  RndStart();
  for (i = 0; i < Anim.NumOfUnits; i++)
    ME3_Anim.Units[i]->Render(ME3_Anim.Units[i], &ME3_Anim);
  RndEnd();
}

VOID ME3_AnimCopyFrame( HDC hDC )
{
  ME3_RndCopyFrame(hDC);
}

VOID ME3_AnimUnitAdd( me3UNIT *Uni )
{
  if (ME3_Anim.NumOfUnits < MAX_UNITS)
    ME3_Anim.Units[ME3_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &ME3_Anim);
}