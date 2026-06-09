/* Eremeev Maksim, 09-3, 06.06.2026 */

#include <math.h>
#include <time.h>
#include "globe.h"
#include "mth.h"

#define pi 3.14159265359
#define GLB_GRID_W 15
#define GLB_GRID_H 30

static DBL GLB_ProjSize = 1, GLB_ProjDist = 2, GLB_Hp, GLB_Wp;/*project plane size*/
static VEC GLB_Geom[GLB_GRID_H][GLB_GRID_W];
static VEC GLB_GeomN[GLB_GRID_H][GLB_GRID_W];
static int GLB_Ws, GLB_Hs;/* frame size*/
static int GLB_A = 25;

COLORREF ColorTo255( VEC Color )
{
  //VEC RGB;
  INT 
    R = (INT)Color.X * 255,
    G = (INT)Color.Y * 255,
    B = (INT)Color.Z * 255;

  if (R < 0)
    R = 0;
  else
    if (R > 255)
      R = 255;

  if (G < 0)
    G = 0;
  else
    if (G > 255)
      G = 255;

  if (B < 0)
    B = 0;
  else
    if (B > 255)
      B = 255;
  return RGB(R, G, B);
}

VEC RotateZ( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * pi / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.X * co - P.Y * si;
  NewP.Y = P.X * si + P.Y * co;
  NewP.Z = P.Z;
  return NewP;
}

VEC RotateX( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * pi / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.X;
  NewP.Y = P.Y * co - P.Z * si;
  NewP.Z = P.Y * si + P.Z * co;
  return NewP;
}

VEC RotateY( VEC P, DBL Angle )
{
  VEC NewP;
  DBL a = Angle * pi / 180, si = sin(a), co = cos(a);
 
  NewP.X = P.Z * si + P.X * co;
  NewP.Y = P.Y;
  NewP.Z = P.Z * co - P.X * si;
  return NewP;
}

VOID GLB_Resize( INT Ws, INT Hs )
{
  GLB_Ws = Ws;
  GLB_Hs = Hs;
  if (GLB_Ws >= GLB_Hs)
    GLB_Wp = GLB_ProjSize * GLB_Ws / GLB_Hs, GLB_Hp = GLB_ProjSize;
  else
    GLB_Wp = GLB_ProjSize, GLB_Hp = GLB_ProjSize * GLB_Hs / GLB_Ws;
}

VOID GLB_Init( DBL R )
{
  int i, j;

  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
    {
      DBL theta = i * pi / GLB_GRID_H, phi = j * 2 * pi / (GLB_GRID_W - 1);
      /* normal*/
      GLB_GeomN[i][j].X = sin(theta) * sin(phi);
      GLB_GeomN[i][j].Y = cos(theta);
      GLB_GeomN[i][j].Z = sin(theta) * cos(phi);
      /*globus*/
      GLB_Geom[i][j].X = 1.2 * R * sin(theta) * sin(phi);
      GLB_Geom[i][j].Y = R * cos(theta);
      GLB_Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
} 

VOID GLB_Draw( HDC hDC )
{
  int i, j, s = 5;
  VEC  L;
  DBL len, si, t = clock() / (DBL)CLOCKS_PER_SEC;
  static POINT pnts[GLB_GRID_H][GLB_GRID_W];  /*<-- массив с проекциями точек*/

  GLB_Init(0.3);
  si = sin(t);
  for (i = 0; i < GLB_GRID_H; i++)
  {
    for (j = 0; j < GLB_GRID_W; j++)
    {
      VEC P = GLB_Geom[i][j];
      DBL xp, yp;

      /*rotate*/
      P = RotateZ(P, (GLB_A * si * 5));
      P  = RotateY(P, GLB_A * si);
      P = RotateX(P, GLB_A * si * 3);

      P.Z -= 3;

      /*project point to plane*/
      xp = P.X * GLB_ProjDist / (-P.Z);
      yp = P.Y * GLB_ProjDist / (-P.Z);


      pnts[i][j].x = (INT)(xp * GLB_Ws / GLB_Wp + GLB_Ws / 2);
      pnts[i][j].y = (INT)(-yp * GLB_Hs / GLB_Hp + GLB_Hs / 2);
    }
  }
  //SelectObject(hDC, GetStockObject(NULL_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 255, 0));
    
  for (i = 0; i < GLB_GRID_H; i++)
    for (j = 0; j < GLB_GRID_W; j++)
      Ellipse(hDC, pnts[i][j].x - s, pnts[i][j].y - s, pnts[i][j].x + s, pnts[i][j].y + s);

  SelectObject(hDC, GetStockObject(NULL_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));

  SetDCPenColor(hDC, RGB(255, 0, 0));

  for (i = 0; i < GLB_GRID_H; i++)
  {
    MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
    for (j = 0; j < GLB_GRID_W; j++)
    {
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
    }
  }

  for (j = 0; j < GLB_GRID_W; j++)
  {
    MoveToEx(hDC, pnts[0][j].x, pnts[0][j].y, NULL);
    for (i = 0; i < GLB_GRID_H; i++)
    {
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
    }
  }

  /*facets*/
  L.X = 1;
  L.Y = 1;
  L.Z = 1;

  /*normalize*/
  len = sqrt(L.X * L.X + L.Y * L.Y + L.Z * L.Z);
  L.X /= len;
  L.Y /= len;
  L.Z /= len;

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(250, 255, 0));
  for (i = 0; i < GLB_GRID_H - 1; i++)
    for (j = 0; j < GLB_GRID_W - 1; j++)
    {
      VEC N = GLB_GeomN[i][j];
      DBL NL;
      POINT pts[4];
      VEC C = {0.9, 0.2, 0.1};/* color*/

      /*N = RotateZ(N, GLB_A * sin(t) * 5);
      N = RotateY(N, GLB_A * sin(t));
      N = RotateX(N, GLB_A * sin(t) * 3);*/

      NL = N.X * L.X + N.Y * L.Y + N.Z * L.Z;/*cos(N^L)*/
      if (NL <= 0.30)
        NL = 0.30;

      C.X *= NL;
      C.Y *= NL;
      C.Z *= NL;

      pts[0] = pnts[i][j];
      pts[1] = pnts[i][j + 1];
      pts[2] = pnts[i + 1][j + 1];
      pts[3] = pnts[i + 1][j];
      SetDCBrushColor(hDC, ColorTo255(C));

      if (((pts[0].x - pts[1].x) * (pts[0].y + pts[1].y) + (pts[1].x - pts[2].x) * (pts[1].y + pts[2].y) + (pts[2].x - pts[3].x) * (pts[2].y + pts[3].y) + (pts[3].x - pts[0].x) * (pts[3].y + pts[0].y)) <= 0)
        Polygon(hDC, pts, 4);
    }
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(255, 255, 255));
}