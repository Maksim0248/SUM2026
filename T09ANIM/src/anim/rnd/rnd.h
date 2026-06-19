/* FILE NAME: rnd.h
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include "OpenGL/glew.h"


#include "res/rndres.h"

extern HWND ME3_hRndWnd;
extern HDC ME3_hRndDC;

extern INT ME3_RndFrameW;
extern INT ME3_RndFrameH;
extern DBL ME3_RndProjSize, ME3_RndProjDist, ME3_RndProjFarClip;

extern MATR ME3_RndMatrView, ME3_RndMatrProj, ME3_RndMatrVP;
extern HGLRC ME3_hRndGLRC; /* Open GL rendering context*/
extern VEC ME3_RndCamLoc, ME3_RndCamAt, ME3_RndCamDir, ME3_RndCamRight, ME3_RndCamUp;

VOID ME3_RndInit( HWND hWnd );
VOID ME3_RndClose( VOID );
VOID ME3_RndStart( VOID );
VOID ME3_RndResize( INT W, INT H );
VOID ME3_RndProjSet( VOID );
VOID ME3_RndCamSet( VEC Loc, VEC At, VEC Up );
VOID ME3_RndEnd( VOID );
VOID ME3_RndCopyFrame( VOID );

/*** Primitives ***/
typedef struct tagme34VERTEX
 {
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
} me3VERTEX;

typedef enum tagme3PRIM_TYPE
{
  ME3_RND_PRIM_POINTS,   /* Array of points  – GL_POINTS */
  ME3_RND_PRIM_LINES,    /* Line segments (by 2 points) – GL_LINES */
  ME3_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles – GL_TRIANGLES */
  ME3_RND_PRIM_TRISTRIP,
} me3PRIM_TYPE;



/* Primitive representation type */
typedef struct tagme3PRIM
{
  me3PRIM_TYPE Type; /* Primitive type */
 
  INT
    VA,              /* Vertex array Id */
    VBuf,            /* Vertex buffer Id */
    IBuf;            /* Index buffer Id (if 0 - use only vertex buffer) */
 
  INT NumOfElements; /* Number of indices/vecrtices */
 
  VEC MinBB, MaxBB;  /* Bound box */
 
  MATR Trans;   /* Additional transformation matrix */
  INT MtlNo;
} me3PRIM;

typedef struct tagme3PRIMS
{
  INT NumOfPrims; /* Number of primitives in array */  
  me3PRIM *Prims; /* Array of primitives */
  MATR Trans;     /* Common transformation matrix */
} me3PRIMS;

/* Grid topology representation type */
typedef struct tagme3GRID
{
  INT W, H;      /* Grid size (in vertices) */
  me3VERTEX *V;  /* Array (2D) of vertex */
} me3GRID;

VOID ME3_RndPrimFree( me3PRIM *Pr );
VOID ME3_RndPrimCreate( me3PRIM *Pr, me3PRIM_TYPE Type, me3VERTEX *V, INT NoofV, INT *Ind, INT NoofI );
VOID ME3_RndPrimDraw( me3PRIM *Pr, MATR World );
BOOL ME3_RndPrimCreateSphere( me3PRIM *Pr, DBL R, INT W, INT H );
BOOL ME3_RndPrimLoad( me3PRIM *Pr, CHAR *FileName );
VOID APIENTRY glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity, INT Length, const CHAR *Message,
                             const VOID *UserParam );
BOOL ME3_RndGridCreateSphere( me3GRID *G, FLT R, INT W, INT H );
BOOL ME3_RndPrimFromGrid( me3PRIM *Pr, me3GRID *G );
VOID ME3_RndGridFree( me3GRID *G );
#endif /* __rnd_h_ */
