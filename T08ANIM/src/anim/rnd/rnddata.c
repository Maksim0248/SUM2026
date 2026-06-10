/* FILE NAME: rnddata.c
 * PURPOSE: 3D render data module.
 * PROGRAMMER: ME3
 * DATE: 10.06.2026
 */

#include "def.h"

HWND ME3_hRndWnd;
HDC ME3_hRndDCFrame;

INT ME3_RndFrameW;
INT ME3_RndFrameH;
DBL ME3_RndProjSize = 0.1, ME3_RndProjDist = 0.1, ME3_RndProjFarClip = 300;
HBITMAP ME3_hRndBmFrame;

MATR ME3_RndMatrView, ME3_RndMatrProj, ME3_RndMatrVP;
