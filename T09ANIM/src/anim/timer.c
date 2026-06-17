/* FILE NAME: timer.c
 * PURPOSE: -
 * PROGRAMMER: ME3
 * DATE: 08.06.2026
 */

#include "anim.h"

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
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  ME3_Anim.IsPause = FALSE;
  ME3_Anim.FPS = 30.0;
  PauseTime = 0;
}

VOID TimerResponse( VOID )
{
  LARGE_INTEGER t;
 
  QueryPerformanceCounter(&t);
 
  /* Global time */
  ME3_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec; /*time from start of project*/
  ME3_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec; /*time from past clip*/
  
  /* Time with pause */
  if (ME3_Anim.IsPause)
  {

    ME3_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    ME3_Anim.DeltaTime = ME3_Anim.GlobalDeltaTime;
    ME3_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime); /*time of work*/
  }
 
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > 3 * TimePerSec) /*once in 3 second show FPS*/
  {
    ME3_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}