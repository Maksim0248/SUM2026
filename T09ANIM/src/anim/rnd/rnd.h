/* FILE NAME: rnd.h
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include "def.h"

extern HWND ME3_hRndWnd;
extern HDC ME3_hRndDC;

extern INT ME3_RndFrameW;
extern INT ME3_RndFrameH;
extern DBL ME3_RndProjSize, ME3_RndProjDist, ME3_RndProjFarClip;

extern MATR ME3_RndMatrView, ME3_RndMatrProj, ME3_RndMatrVP;
extern HGLRC ME3_hRndGLRC; /* Open GL rendering context*/

VOID ME3_RndInit( HWND hWnd );
VOID ME3_RndClose( VOID );
VOID ME3_RndStart( VOID );
VOID ME3_RndResize( INT W, INT H );
VOID ME3_RndProjSet( VOID );
VOID ME3_RndCamSet( VEC Loc, VEC At, VEC Up );
VOID ME3_RndEnd( VOID );
VOID ME3_RndCopyFrame( VOID );

/*** Primitives ***/
typedef struct tagme3VERTEX 
{
  VEC P;
  VEC4 C;
} me3VERTEX;

typedef struct tagme3PRIM 
{
  me3VERTEX *V; 
  INT NumOfV;
 
  INT *I;
  INT NumOfI;
 
  MATR Trans;
} me3PRIM;

VOID ME3_RndPrimFree( me3PRIM *Pr );
BOOL ME3_RndPrimCreate( me3PRIM *Pr, INT NoofV, INT NoofI );
VOID ME3_RndPrimDraw( me3PRIM *Pr, MATR World );
BOOL ME3_RndPrimCreateSphere( me3PRIM *Pr, DBL R, INT W, INT H );
BOOL ME3_RndPrimLoad( me3PRIM *Pr, CHAR *FileName );


#endif /* __rnd_h_ */
