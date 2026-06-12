/* FILE NAME: anim.h
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define me3_MAX_UNITS 1000

#define me3UNIT_BASE_FIELDS \
  VOID (*Init)( me3UNIT *Uni, me3ANIM *Ani ); \
  VOID (*Close)( me3UNIT *Uni, me3ANIM *Ani ); \
  VOID (*Response)( me3UNIT *Uni, me3ANIM *Ani ); \
  VOID (*Render)( me3UNIT *Uni, me3ANIM *Ani ) 

typedef struct tagme3ANIM me3ANIM;
typedef struct tagme3UNIT me3UNIT; 
struct tagme3UNIT
{
  me3UNIT_BASE_FIELDS;
};

#define MAX_UNITS 3000
me3UNIT * Units[me3_MAX_UNITS];
INT NumOfUnits;

typedef struct tagme3ANIM
{
  HWND hWnd;                      
  HDC hDC;                        
  INT W, H;                       
 
  me3UNIT * Units[me3_MAX_UNITS];
  INT NumOfUnits;
  DBL
    GlobalTime, GlobalDeltaTime,
    Time, DeltaTime,
    FPS;
  BOOL
    IsPause;
  BYTE Keys[256]; /*state of buttons on this frame*/
  BYTE KeysClick[256]; /* 1 click state*/
};

extern me3ANIM ME3_Anim; 

VOID ME3_AnimInit( hWnd );
VOID ME3_AnimResize( INT W, INT H );
VOID ME3_AnimRender( VOID );
VOID ME3_AnimUnitAdd( me3UNIT *Uni );
VOID ME3_AnimClose( VOID );
me3UNIT * ME3_AnimUnitCreate( INT Size );
VOID ME3_AnimCopyFrame( HDC hDC );
VOID TimerResponse( VOID );
VOID TimerInit( VOID );
VOID ME3_AnimInputInit( VOID );
VOID ME3_AnimInputResponse( VOID );

#endif /* __anim_h_ */