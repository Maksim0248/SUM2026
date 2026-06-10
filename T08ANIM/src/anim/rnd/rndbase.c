/* FILE NAME: rnd.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#include "rnd.h"

VOID ME3_RndInit( HWND hWnd )
{

  ME3_hRndWnd = hWnd;
  ME3_hRndDCFrame = CreateCompatibleDC(GetDC(hWnd));
}

VOID ME3_RndClose( VOID )
{
  KillTimer(ME3_hRndWnd, 30);
  ReleaseDC(ME3_hRndWnd, ME3_hRndDCFrame);
}

VOID ME3_RndStart( VOID )
{
  VEC p1, p2, p;
  MATR m;
  POINT pnts[2];
  Rectangle(ME3_hRndDCFrame, 0, 0, ME3_RndFrameW, ME3_RndFrameH);
  /*SelectObject(ME3_hRndDCFrame, GetStockObject(BLACK_BRUSH));
  
  Rectangle(ME3_hRndDCFrame, 50, 50, 10, 10);
  SelectObject(ME3_hRndDCFrame, GetStockObject(WHITE_BRUSH));*/
 
  ME3_RndCamSet(VecSet(5, 10, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  m = ME3_RndMatrVP;
 
  p1 = VecSet(0, 0, 0);
  p2 = VecSet(1, 0, 0);
 
  p = VecMulMatr(p1, m);
  pnts[0].x = (INT)((p.X + 1) * ME3_RndFrameW / 2);
  pnts[0].y = (INT)((-p.Y + 1) * ME3_RndFrameH / 2);
 
  p = VecMulMatr(p2, m);
  pnts[1].x = (INT)((p.X + 1) * ME3_RndFrameW / 2);
  pnts[1].y = (INT)((-p.Y + 1) * ME3_RndFrameH / 2);
 
  MoveToEx(ME3_hRndDCFrame, pnts[0].x, pnts[0].y, NULL);
  LineTo(ME3_hRndDCFrame, pnts[1].x, pnts[1].y);
}



VOID ME3_RndEnd( VOID )
{
}

VOID ME3_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, ME3_RndFrameW, ME3_RndFrameH,
    ME3_hRndDCFrame, 0, 0, SRCCOPY);
}

VOID ME3_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(ME3_hRndWnd);
 
  if (ME3_hRndBmFrame != NULL)
    DeleteObject(ME3_hRndBmFrame);
  ME3_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(ME3_hRndWnd, hDC);
 
  SelectObject(ME3_hRndDCFrame, ME3_hRndBmFrame);
 
  /* сохран€ем размеры */
  ME3_RndFrameW = W;
  ME3_RndFrameH = H;
 
  /* пересчитываем проекцию */
  ME3_RndProjSet();
}

VOID ME3_RndProjSet( VOID )
{
  DBL rx, ry;
 
  rx = ry = ME3_RndProjSize;
  /* Correct aspect ratio */
  if (ME3_RndFrameW > ME3_RndFrameH)
    rx *= (DBL)ME3_RndFrameW / ME3_RndFrameH;
  else
    ry *= (DBL)ME3_RndFrameH / ME3_RndFrameW;

  ME3_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2, ME3_RndProjDist, ME3_RndProjFarClip);
  ME3_RndMatrVP = MatrMulMatr(ME3_RndMatrView, ME3_RndMatrProj);
}
 
VOID ME3_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  ME3_RndMatrView = MatrView(Loc, At, Up);
  ME3_RndMatrVP = MatrMulMatr(ME3_RndMatrView, ME3_RndMatrProj);
}