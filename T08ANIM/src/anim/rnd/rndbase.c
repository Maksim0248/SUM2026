/* FILE NAME: rnd.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#include "rnd.h"
#include <string.h>

VOID ME3_RndInit( HWND hWnd )
{
  
  ME3_hRndWnd = hWnd;
  ME3_hRndDCFrame = CreateCompatibleDC(GetDC(hWnd));

  ME3_hRndBmFrame = NULL;
  ME3_RndCamSet(VecSet(10, 20, 30), VecSet(0, 0, 0), VecSet(0, 1, 0));

}

VOID ME3_RndClose( VOID )
{
  KillTimer(ME3_hRndWnd, 30);
  ReleaseDC(ME3_hRndWnd, ME3_hRndDCFrame);
}

VOID ME3_RndStart( VOID )
{
  //me3PRIM Pr, Pr1, Pr2;
  Rectangle(ME3_hRndDCFrame, 0, 0, ME3_RndFrameW, ME3_RndFrameH);
  
  /*if (ME3_RndPrimCreate(&Pr, 4, 6))
  {
    Pr.V[0].P = VecSet(0, 0, 0);
    Pr.V[1].P = VecSet(2, 0, 0);
    Pr.V[2].P = VecSet(0, 2, 0);
    Pr.V[3].P = VecSet(2, 2, 0);

    Pr.I[0] = 0;
    Pr.I[1] = 1;
    Pr.I[2] = 2;

    Pr.I[3] = 2;
    Pr.I[4] = 1;
    Pr.I[5] = 3;
  }
  ME3_RndPrimDraw(&Pr, MatrIdentity());

  ME3_RndPrimCreateSphere(&Pr1, 1, 25, 50);
  ME3_RndPrimDraw(&Pr1, MatrIdentity());

  ME3_RndPrimLoad(&Pr2, "bin/models/cow.obj");
  ME3_RndPrimDraw(&Pr2, MatrIdentity());
  ME3_RndPrimFree(&Pr);
  ME3_RndPrimFree(&Pr1);
  ME3_RndPrimFree(&Pr2);*/
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
 
  ME3_RndFrameW = W;
  ME3_RndFrameH = H;
 
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
/*End*/