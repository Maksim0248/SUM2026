/* Eremeev Maksim, 09-3, 06.06.2026 */

#include <windows.h>
#include <math.h>
#include "def.h"

#define WND_CLASS_NAME "cgsg"

#include "units/units.h"
#include <stdio.h>
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
 
  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO mi;
    RECT rc;
 
    /* Save old window size and position */
    GetWindowRect(hWnd, &SaveRect);

    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    mi.cbSize = sizeof(mi);
    GetMonitorInfo(hmon, &mi);

    /* Go to full screen mode */
    rc = mi.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
  
    /* Expand window */
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left,
      rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  IsFullScreen = TRUE;
  }
  else
  {
    /* Restore from full screen mode */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left,
      SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  CONSOLE_FONT_INFOEX cfi = {0};
  HWND hConWnd;
  SetDbgMemHooks();
 
  /* Create console */
  AllocConsole();
 
  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
  cfi.dwFontSize.Y = 18;
  cfi.FontWeight = FW_BOLD;
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
 
  freopen("CONOUT$", "w", stdout);
  system("@chcp 1251 > nul");
  printf("\x1b[38;2;%d;%d;%dm \x1b[48;2;%d;%d;%dm", 255, 255, 0, 0, 102, 102);
                  /*RGB символов*/      /*RGB фона*/
  printf("Группа компьютерной графики ФМЛ № 30\n");
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 0, 255, 0, 0, 0, 0);
  printf("Computer Graphics Support Group\n");
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm", 0, 0, 255, 0, 0, 0);
  fflush(stdout);
 
  hConWnd = GetConsoleWindow();
  /*MoveWindow(hConWnd, 0, 0, 1920, 1080, FALSE);
  SetWindowPos(hConWnd, HWND_TOP, 0, 0, 1920, 1000, 0);*/


  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error", "ERROR", MB_YESNO | MB_ICONSTOP);
    return 0;
  }
  hWnd = CreateWindow(WND_CLASS_NAME, "SummerPractice2026", WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  /*--> WM_CREATE --> WM_SIZE -->*/


  UpdateWindow(hWnd);

  ME3_AnimUnitAdd(ME3_UnitCreateBall());
  ME3_AnimUnitAdd(ME3_UnitCreateCam());
  /*ME3_AnimUnitAdd(ME3_UnitCreateCow());*/

  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 30, 0);
  }
  return msg.wParam;
}


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  static SYSTEMTIME st;
  static INT W, H;
  MINMAXINFO *minmax;

  static int Xc, Yc, lenm = 200, lenh = 100, x, y;

  static HDC hDC;
  
  switch (Msg)
  {
  case WM_CREATE:
    ME3_AnimInit(hWnd);
    SetTimer(hWnd, 30, 10, NULL);
    return 0;

  case WM_SIZE: /* after create */
    ME3_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 30, 0);
    return 0;

  case WM_TIMER:
    ME3_AnimRender();
    ME3_AnimCopyFrame();
    return 0;

  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    minmax->ptMaxTrackSize.x = 2000;
    return 0;
  case WM_COMMAND:
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_KEYDOWN:
    if (wParam == VK_F11)
      FlipFullScreen(hWnd);
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_DESTROY, 0, 0);
  case WM_MOUSEMOVE:
    return 0;

  case WM_DESTROY:
    ME3_AnimClose();
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}