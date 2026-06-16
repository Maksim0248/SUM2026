/* Eremeev Maksim, 09-3, 03.06.2026 */

#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "cgsg"
#define PI 3.14159265359
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

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
 
  CreateWindow("BUTTON", "Close window", WS_CHILD | WS_VISIBLE,
    10, 10, 100, 30, hWnd, (HMENU)123, hInstance, NULL);

  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

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

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  static SYSTEMTIME st;
  static INT W, H;
  char tx[50];
  CHAR Buf[100];
  MINMAXINFO *minmax;

  static int Xc, Yc, lenm = 200, lenh = 100, x, y;
  static BITMAP bm;
  double angle;

  /*Descriptors*/
  static HDC hDC;/*контекст окна*/

  static HDC hMemDC; /*контекст растового изображения в памяти*/
  static HBITMAP hBm; /*растовое изображение(буффер) в памяти*/

  static HBITMAP hClBm; /*растовое изображение часов в памяти*/
  static HDC hMemClDC; /*контекст растового изображения в памяти*/
 
  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);/*копия контекста экрана*/
    hMemClDC = CreateCompatibleDC(hDC);
    SetTimer(hWnd, 30, 10, NULL);
    if ((hClBm = LoadImage(NULL, "me3_clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) == NULL)
    {
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    }
    GetObject(hClBm, sizeof(BITMAP), &bm);
    SelectObject(hMemClDC, hClBm);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_SIZE:/* идет сразу после create */
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    Xc = W / 2 - 2;
    Yc = H / 2 + 5;
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);/*функция создает картинку в памяти*/
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 30, 0);

    ReleaseDC(hWnd, hDC);

    return 0;

  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    minmax->ptMaxTrackSize.x = 2000;
    return 0;
  case WM_TIMER:
    hDC = GetDC(hWnd);
    Rectangle(hMemDC, 0, 0, W, H);
    BitBlt(hMemDC, (W - bm.bmWidth) / 2, (H - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hMemClDC, 0, 0, SRCCOPY);
    SetStretchBltMode(hMemDC, COLORONCOLOR);
    StretchBlt(hMemDC, W / 4, H / 8, W / 2, H / 1.3, hMemClDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    GetLocalTime(&st);
    TextOut(hMemDC, W / 2.2, H / 1.1, Buf, sprintf(Buf, ">>> time: %d:%d:%d <<<", st.wHour, st.wMinute, st.wSecond));
    TextOut(hMemDC, W / 1.15, H / 1.05, Buf, sprintf(Buf, ">>> data: %d.%d.%d <<<", st.wDay, st.wMonth, st.wYear));
    /*Draw arrow*/
    /*sec*/
    angle = st.wSecond * 2 * PI / 60;
    x = Xc + lenm * sin(angle);
    y = Yc - lenm * cos(angle);
    MoveToEx(hMemDC, Xc, Yc, NULL);
    //SetDCPenColor(hDC, RGB(200, 200, 0));
    /*POINT pts[4] = {{Xc, Yc}, {4, -10}, {4, 6}, {10, 6}, {0, 14}, {-10, 6}, {-4, 6}};
    Polygon(hMemDC,*/ 
    LineTo(hMemDC, x, y);
    /*minute*/
    angle = (st.wMinute + st.wSecond / 60.0) * 2 * PI / 60;
    x = Xc + (lenm - 10) * sin(angle);
    y = Yc - (lenm - 10) * cos(angle);
    MoveToEx(hMemDC, Xc, Yc, NULL);
    LineTo(hMemDC, x, y);
    /* hour */
    angle = (st.wHour % 12 + st.wMinute / 60.0) * 2 * PI / 12;
    x = Xc + lenh * sin(angle);
    y = Yc - lenh * cos(angle);
    MoveToEx(hMemDC, Xc, Yc, NULL);
    LineTo(hMemDC, x, y);

    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_F11)
      FlipFullScreen(hWnd);
  case WM_COMMAND:
    if (LOWORD(wParam) == 123)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;

  case WM_MOUSEMOVE:
    /*hDC = GetDC(hWnd);
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W, H);
    
    ReleaseDC(hWnd, hDC);*/
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure close window?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 30);
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
