/* FILE NAME: anim.h
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define MAX_UNITS 3000
UNIT * Units[me3_MAX_UNITS];
INT NumOfUnits;

#define UNIT_BASE_FIELDS \
  VOID (*Init)( UNIT *Uni, ANIM *Ani ); \
  VOID (*Close)( UNIT *Uni, ANIM *Ani ); \
  VOID (*Response)( UNIT *Uni, ANIM *Ani ); \
  VOID (*Render)( UNIT *Uni, ANIM *Ani ) 
 
struct tagme3UNIT
{
  UNIT_BASE_FIELDS;
};

typedef struct tagme3UNIT me3UNIT;
typedef struct tagme3ANIM
{
  HWND hWnd;                      
  HDC hDC;                        
  INT W, H;                       
 
  me3UNIT * Units[me3_MAX_UNITS]; 
  INT NumOfUnits;                 
  /*DBL
    GlobalTime, GlobalDeltaTime, 
    Time, DeltaTime,             
    FPS;                         
  BOOL
    IsPause;                     */
};
typedef struct tagme3ANIM me3ANIM;

extern tagme3ANIM ME3_Anim; /* контекст анимации*/

VOID ME3_AnimInit(hWnd);
VOID ME3_AnimResize( INT W, INT H );
VOID ME3_AnimRender( VOID );
VOID ME3_AnimUnitAdd( UNIT *Uni );

#endif /* __anim_h_ */