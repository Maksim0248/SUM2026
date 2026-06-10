/* FILE NAME: rnd.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#ifndef __rnd_h_
#define __rnd_h_

#include "def.h"

extern HWND ME3_hRndWnd;
extern HDC ME3_hRndDCFrame;

extern INT ME3_RndFrameW;
extern INT ME3_RndFrameH;
extern DBL ME3_RndProjSize, ME3_RndProjDist, ME3_RndProjFarClip;
extern HBITMAP ME3_hRndBmFrame;

extern MATR ME3_RndMatrView, ME3_RndMatrProj, ME3_RndMatrVP;


VOID ME3_RndInit( HWND hWnd );
VOID ME3_RndClose( VOID );
VOID ME3_RndStart( VOID );
VOID ME3_RndResize( INT W, INT H );
VOID ME3_RndProjSet( VOID );
VOID ME3_RndCamSet( VEC Loc, VEC At, VEC Up );
VOID ME3_RndEnd( VOID );
VOID ME3_RndCopyFrame( HDC hDC );

/*** Primitives ***/

typedef struct tagme3VERTEX
{
  VEC P;
} vg4VERTEX;

typedef struct tagme3PRIM
{
  vg4VERTEX *V; 
  INT NumOfV;
 
  INT *I;
  INT NumOfI;
 
  MATR Trans;
} vg4PRIM;

#endif /* __rnd_h_ */
