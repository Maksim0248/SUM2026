/* FILE NAME: rnd.h
 * PURPOSE: 3D math implementation module.
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#include "rnd.h"
#include <string.h>
#include "OpenGl/wglew.h"
#include "OpenGl/wglext.h"

#pragma comment(lib, "opengl32")


VOID ME3_RndInit( HWND hWnd )
{ 
  INT i, nums;
  HGLRC hRC;
  PIXELFORMATDESCRIPTOR pfd = {0};

  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };
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

  wglChoosePixelFormatARB(ME3_hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(ME3_hRndDC, NULL, ContextAttribs);
 
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(ME3_hRndGLRC);
 
  ME3_hRndGLRC = hRC;
  wglMakeCurrent(ME3_hRndDC, ME3_hRndGLRC);

  #ifndef NDEBUG
    OutputDebugString(glGetString(GL_VERSION));
    OutputDebugString("\n");
    OutputDebugString(glGetString(GL_VENDOR));
    OutputDebugString("\n");
    OutputDebugString(glGetString(GL_RENDERER));
    OutputDebugString("\n");
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, NULL);/*func from rnddebug.c*/
#endif /* NDEBUG */

  /* Render parameters setup */
  glEnable(GL_DEPTH_TEST);
  wglSwapIntervalEXT(0);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  ME3_RndProjSize = 0.1;
  ME3_RndProjDist = ME3_RndProjSize;
  ME3_RndProjFarClip = 3000;
  ME3_RndFrameW = 47;
  ME3_RndFrameH = 47;
  
  ME3_RndCamSet(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));

  ME3_RndResInit();
}

VOID ME3_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H); /* area where GL draw */

  ME3_RndFrameW = W;
  ME3_RndFrameH = H;
 
  ME3_RndProjSet();
  //ME3_RndCamSet(VecSet(15, 0, 0), VecSet(0, 0, 0), VecSet(0, 1, 0));
}

VOID ME3_RndStart( VOID )
{
  VEC4 ClearColor = {0.15, 0.1, 0.1, 1};
  FLT DepthClearValue = 1;
  
  ME3_RndShdUpdate();
  /* Clear frame */
  glClearBufferfv(GL_COLOR, 0, &ClearColor.X);
  glClearBufferfv(GL_DEPTH, 0, &DepthClearValue);
}

VOID ME3_RndClose( VOID )
{
  ME3_RndResClose();

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
  ME3_RndCamRight = VecSet(ME3_RndMatrView.A[0][0],
                         ME3_RndMatrView.A[1][0],
                         ME3_RndMatrView.A[2][0]); 
  ME3_RndCamUp = VecSet(ME3_RndMatrView.A[0][1],
                      ME3_RndMatrView.A[1][1],
                      ME3_RndMatrView.A[2][1]);
  ME3_RndCamDir = VecSet(-ME3_RndMatrView.A[0][2],
                       -ME3_RndMatrView.A[1][2],
                       -ME3_RndMatrView.A[2][2]);
  ME3_RndCamLoc = Loc;
  ME3_RndCamAt = At;
}
/*End*/