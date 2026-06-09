/* Eremeev Maksim, 09-3, 06.06.2026 */

#ifndef __globe_h_
#define __globe_h_
#include "mth.h"
#include <windows.h>

typedef double DBL;

VOID GLB_Init( DBL R );
VOID GLB_Draw( HDC hDC );
VOID GLB_Resize( INT W, INT H );

#endif /*__globe_h_*/
