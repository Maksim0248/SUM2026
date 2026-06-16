/* FILE NAME: mth.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#ifndef __mth_h_
#define __mth_h_

#include <math.h>
#include <windows.h>
#include "def.h"

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)
 
/* базовый вещественный тип */
typedef double DBL;
typedef float FLT;

typedef struct
{
  FLT X, Y, Z;
} VEC;

typedef struct
{
  FLT X, Y, Z, W;
} VEC4;

typedef struct
{
  FLT X, Y;
} VEC2;

typedef struct
{
  FLT A[4][4];
} MATR;

__inline VEC VecSet( FLT X, FLT Y, FLT Z )
{
  VEC v;
 
  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
}

__inline VEC4 VecSet4( FLT X, FLT Y, FLT Z, FLT W)
{
  VEC4 v;
 
  v.X = X;
  v.Y = Y;
  v.Z = Z;
  v.W = W;

  return v;
}


__inline VEC VecSet1( FLT A )
{
  VEC r = {A, A, A};
 
  return r;
}
 
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}

__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

__inline VEC VecMulNum( VEC V1, FLT N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

__inline VEC VecDivNum( VEC V1, FLT N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg( VEC V )
 {
  return VecSet(-V.X, -V.Y, -V.Z);
}


__inline FLT VecDotVec( VEC V1, VEC V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
}

__inline FLT VecLen( VEC V )/*|V|*/
{
  FLT len = VecDotVec(V, V);
 
  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
}
__inline FLT VecLen2( VEC V )
{
  FLT len = VecDotVec(V, V);

  return len;
}

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
}

__inline VEC VecNormalize( VEC V )   /*V/|V|*/
{
  FLT len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
}
__inline VEC VecMinVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X < V2.X ? V1.X : V2.X,
                V1.Y < V2.Y ? V1.Y : V2.Y,
                V1.Z < V2.Z ? V1.Z : V2.Z);
} /* End of 'VecMinVec' function */

__inline VEC VecMaxVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X > V2.X ? V1.X : V2.X,
                V1.Y > V2.Y ? V1.Y : V2.Y,
                V1.Z > V2.Z ? V1.Z : V2.Z);
} /* End of 'VecMaxVec' function */


__inline VEC PointTransform( VEC V, MATR M )   /*(V, 1) * M4x3*/
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + 1 * M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + 1 * M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + 1 * M.A[3][2]);
}

__inline VEC VectorTransform( VEC V, MATR M ) /*V * M3x3*/
{ 
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}
__inline VEC VecMulMatr( VEC V, MATR M )       /*V * M4x4 / w*/
{ 
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];
 
  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
         (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
         (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}





/*Matrix*/

/* единичная матрица */
static MATR UnitMatrix =
{
  {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  }
};

__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}

__inline MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
              FLT A10, FLT A11, FLT A12, FLT A13,
              FLT A20, FLT A21, FLT A22, FLT A23,
              FLT A30, FLT A31, FLT A32, FLT A33 )
{
  MATR m =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33},
    }
  };
  return m;
}


__inline MATR MatrTranslate( VEC T )/*перенос*/
{
  return MatrSet(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 T.X, T.Y, T.Z, 1);
}
__inline MATR MatrScale( VEC S )/*умножение*/
{
  return MatrSet(S.X, 0, 0, 0,
                 0, S.Y, 0, 0,
                 0, 0, S.Z, 0,
                 0, 0, 0, 1);
}

__inline MATR MatrRotateZ( FLT AngleInDegree )
{
  AngleInDegree = D2R(AngleInDegree);
  return MatrSet(cos(AngleInDegree), sin(AngleInDegree), 0, 0,
                 -sin(AngleInDegree), cos(AngleInDegree), 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
}

__inline MATR MatrRotateX( FLT AngleInDegree )
{
  AngleInDegree = D2R(AngleInDegree);
  return MatrSet(1, 0, 0, 0,
                 0, cos(AngleInDegree), sin(AngleInDegree), 0,
                 0, -sin(AngleInDegree), cos(AngleInDegree), 0,
                 0, 0, 0, 1);
}

__inline MATR MatrRotateY( FLT AngleInDegree )
{
  FLT co = cos(AngleInDegree);
  FLT si = sin(AngleInDegree);

  AngleInDegree = D2R(AngleInDegree);

  return MatrSet(co, 0, -si, 0,
                 0, 1, 0, 0,
                 si, 0, co, 0,
                 0, 0, 0, 1);
}
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  int i, j, k;
  MATR r = {{{0}}}; /*all = 0*/
 
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}

__inline MATR MatrTranspose( MATR M )
{
  int i, j;
  MATR r = {{{0}}};

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      r.A[i][j] = M.A[j][i];

  return r;
}

__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13, FLT A21, FLT A22, FLT A23, FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 - A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                               M.A[2][0], M.A[2][2], M.A[2][3],
                               M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                               M.A[2][0], M.A[2][1], M.A[2][2],
                               M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrInverse( MATR M ) /* обратная матрица */
{
  FLT det = MatrDeterm(M);
  MATR r;
 
  if (det == 0)
    return MatrIdentity();
 
 
  /* Build adjoint matrix */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
  r.A[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;
 
  r.A[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]) / det;
 
  return r;
}


/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(At, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
    {
      {Right.X, Up.X, -Dir.X, 0},
      {Right.Y, Up.Y, -Dir.Y, 0},
      {Right.Z, Up.Z, -Dir.Z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
} /* End of 'MatrView' function */

/* Perspective (frustum) projection matrix setup function.
 * ARGUMENTS:
 *   - frustum side facets coordinates:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrFrustum( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  MATR m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };

  return m;
} /* End of 'MatrFrustum' function */

__inline FLT Rnd1( VOID )
{
  return 2.0 * rand() / RAND_MAX - 1;
}
__inline FLT R2D( FLT R )
{
  return R * 180.0 / PI;
}
_inline MATR MatrMulMatr3( MATR M1, MATR M2, MATR M3 )
{
  return MatrMulMatr(MatrMulMatr(M1, M2), M3);
}

#endif /*__mth_h_*/