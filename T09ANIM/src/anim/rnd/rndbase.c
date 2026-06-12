/* FILE NAME: rnd.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#include "rnd.h"
#include <string.h>
#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

VOID ME3_RndInit( HWND hWnd )
{ 
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  ME3_hRndWnd = hWnd;

  /* Prepare frame compatible device contesxt */
  ME3_hRndDC = GetDC(hWnd);

    /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(ME3_hRndDC, &pfd);

  DescribePixelFormat(ME3_hRndDC, i, sizeof(pfd), &pfd); /* describe pixels */
  SetPixelFormat(ME3_hRndDC, i, &pfd);

  /* OpenGL init: rendering context setup */
  ME3_hRndGLRC = wglCreateContext(ME3_hRndDC);
  wglMakeCurrent(ME3_hRndDC, ME3_hRndGLRC);

  if (glewInit() != GLEW_OK)
    exit(0);

  /* Render parameters setup */
  glEnable(GL_DEPTH_TEST);
 
  ME3_RndProjSize = 0.1;
  ME3_RndProjDist = ME3_RndProjSize;
  ME3_RndProjFarClip = 300;
  ME3_RndFrameW = 47;
  ME3_RndFrameH = 47;
  
  ME3_RndCamSet(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
}

VOID ME3_RndStart( VOID )
{
  VEC4 ClearColor = {0.2, 0.7, 0.1, 1};
  FLT DepthClearValue = 1;
 
  /* Clear frame */
  glClearBufferfv(GL_COLOR, 0, &ClearColor.X);
  glClearBufferfv(GL_DEPTH, 0, &DepthClearValue);
}

VOID ME3_RndClose( VOID )
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(ME3_hRndGLRC);
  KillTimer(ME3_hRndWnd, 30);
  ReleaseDC(ME3_hRndWnd, ME3_hRndDC);
}

VOID ME3_RndEnd( VOID )
{
  glFinish();
}

VOID ME3_RndCopyFrame( VOID )
{
  SwapBuffers(ME3_hRndDC);
}

VOID ME3_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H); /* area where we draw */

  ME3_RndFrameW = W;
  ME3_RndFrameH = H;
 
  ME3_RndProjSet();
  ME3_RndCamSet(VecSet(10, 20, 30), VecSet(0, 0, 0), VecSet(0, 1, 0));
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