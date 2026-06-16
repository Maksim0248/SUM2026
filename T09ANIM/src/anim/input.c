/* FILE NAME: input.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 12.06.2026
 */

#include "anim.h"

static BYTE OldKeys[256];
INT ME3_MouseWheel;

static VOID KeyboardInit( VOID )
{
  INT i;

  GetKeyboardState(ME3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    ME3_Anim.Keys[i] >>= 7;
  }
  memcpy(OldKeys, ME3_Anim.Keys, 256);
}

static VOID MouseInit( VOID )
{
  POINT pt;
 
  GetCursorPos(&pt);
  ScreenToClient(ME3_Anim.hWnd, &pt);

  ME3_Anim.Mdx = ME3_Anim.Mdy = 0;

  ME3_Anim.Mx = pt.x;
  ME3_Anim.My = pt.y;

  ME3_MouseWheel = ME3_Anim.Mdz = ME3_Anim.Mz = 0;
}

static VOID KeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(ME3_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    ME3_Anim.Keys[i] >>= 7;
    ME3_Anim.KeysClick[i] = ME3_Anim.Keys[i] && !OldKeys[i];
  }
  memcpy(OldKeys, ME3_Anim.Keys, 256);
}

static VOID MouseResponse( VOID )
{
  POINT pt;
 
  GetCursorPos(&pt);
  ScreenToClient(ME3_Anim.hWnd, &pt);
 
  /* приращение координат на экране */
  ME3_Anim.Mdx = pt.x - ME3_Anim.Mx;
  ME3_Anim.Mdy = pt.y - ME3_Anim.My;
  /* абсолютные значения */
  ME3_Anim.Mx = pt.x;
  ME3_Anim.My = pt.y;

  ME3_Anim.Mdz = ME3_MouseWheel;
  ME3_Anim.Mz += ME3_MouseWheel;
  ME3_MouseWheel = 0;
}

VOID ME3_AnimInputInit( VOID )
{
  KeyboardInit();
  MouseInit();
}
VOID ME3_AnimInputResponse( VOID )
{
  KeyboardResponse();
  MouseResponse();
}
