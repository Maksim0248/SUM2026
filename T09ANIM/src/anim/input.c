/* FILE NAME: input.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 12.06.2026
 */

#include "anim.h"

static BYTE OldKeys[256];

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

VOID ME3_AnimInputInit( VOID )
{
  KeyboardInit();
}
VOID ME3_AnimInputResponse( VOID )
{
  KeyboardResponse();
}
