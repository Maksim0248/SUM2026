/* FILE NAME: timer.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include "def.h"

static UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */

VOID TimerInit( VOID )
{
  LARGE_INTEGER t;
  
  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  //StartTime = OldTime = OldT
}

VOID TimerResponse( VOID )
{}