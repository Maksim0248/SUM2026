/* FILE NAME: anim.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */


#include "anim.h"
#include <string.h>

me3ANIM ME3_Anim; 

VOID ME3_AnimInit( HWND hWnd )
{
  memset(&ME3_Anim, 0, sizeof(me3ANIM));

  ME3_Anim.hWnd = hWnd;
  ME3_RndInit(hWnd);
  ME3_Anim.hDC = ME3_hRndDC;
  TimerInit();
  ME3_AnimInputInit();
}

VOID ME3_AnimClose( VOID )
{
  int i;

  for (i = 0; i < ME3_Anim.NumOfUnits; i++)
  {
    ME3_Anim.Units[i]->Close(ME3_Anim.Units[i], &ME3_Anim);
    free(ME3_Anim.Units[i]);
    ME3_Anim.Units[i] = NULL;
  }
  ME3_RndClose();
}

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

  TimerResponse();
  ME3_AnimInputResponse();
  for (i = 0; i < ME3_Anim.NumOfUnits; i++)
    ME3_Anim.Units[i]->Response(ME3_Anim.Units[i], &ME3_Anim);

  ME3_RndStart();
  for (i = 0; i < ME3_Anim.NumOfUnits; i++)
    ME3_Anim.Units[i]->Render(ME3_Anim.Units[i], &ME3_Anim);
  ME3_RndEnd();
}

VOID ME3_AnimCopyFrame( VOID )
{
  ME3_RndCopyFrame();
}

VOID ME3_AnimUnitAdd( me3UNIT *Uni )
{
  if (ME3_Anim.NumOfUnits < MAX_UNITS)
    ME3_Anim.Units[ME3_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &ME3_Anim);
}