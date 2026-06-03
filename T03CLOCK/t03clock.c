/* Eremeev Maksim, 09-3, 03.06.2026 */

#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "cgsg"
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
  CreateWindow("BUTTON", "Expand", WS_CHILD | WS_VISIBLE,
    10, 47, 100, 30, hWnd, (HMENU)124, hInstance, NULL);
  CreateWindow("BUTTON", "Select me", BS_CHECKBOX | WS_CHILD | WS_VISIBLE | WS_BORDER,
    10, 84, 100, 30, hWnd, (HMENU)125, hInstance, NULL);

  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

void DrawArrow( HWND hWnd, HDC hDC, INT Xc, INT Yc, INT L, INT W, INT H )
{
  POINT pt;
  INT i;
  double x, y, len;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  len = hypot(pt.x - Xc, pt.y - Yc);
  x = Xc + (pt.x - Xc)* L / len;
  y = Yc + (pt.y - Yc)* L / len;
  MoveToEx(hDC, Xc, Yc, NULL);
  LineTo(hDC, x, y);
}

void DrawArrow2( HWND hWnd, HDC hDC, INT Xc, INT Yc, INT L, INT W, INT H )
{
  POINT pts[7] = {{-4, 0}, {0, -4}, {8, 4}, {10, 2}, {12, 12}, {2, 10}, {4, 8}};
  POINT pts_res[sizeof(pts) / sizeof(pts[0])]; 
  int N = sizeof(pts) / sizeof(pts[0]);
  POINT pt;
  INT i;
  DOUBLE x, y, len;


  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  len = hypot(pt.x - Xc, pt.y - Yc);
  x = Xc + (pt.x - Xc)* L / len;
  y = Yc + (pt.y - Yc)* L / len;
  /*MoveToEx(hDC, Xc, Yc, NULL);
  LineTo(hDC, x, y);*/
  for (i = 0; i < N; i++)
  {
    pts_res[i].x = Xc + pts[i].x * (pt.y - Yc)/len + pts[i].y * (pt.x - Xc)/len;
    pts_res[i].y = Yc + pts[i].y * (pt.y - Yc)/len - pts[i].x * (pt.x - Xc)/len;
  }
  Polygon(hDC, pts_res, N);
}



LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  POINT pt;
  SYSTEMTIME st;
  CHAR Buf[100];
  static BOOL is_select = FALSE;
  static INT W, H;
  INT i;
  /*Descriptors*/
  static HDC hDC;
  static HDC hMemDC;
  static HBITMAP hBm;
  static HBITMAP hClBm;
  static HBITMAP hMemClBm;
  static HDC hMemClDC;
  static BITMAP bm;

  /*BITMAPINFOHEADER*/ 
  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);/*copy dc*/
    hMemClDC = CreateCompatibleDC(hDC);/*copy dc*/
    SetTimer(hWnd, 30, 10, NULL);
    if ((hClBm = LoadImage(NULL, "me3_clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) == NULL)
    {
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    }
    GetObject(hClBm, sizeof(BITMAP), &bm);
    SelectObject(hMemClDC, hMemClBm);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);

    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, FALSE);
    BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemClDC, 0, 0, SRCCOPY);
    return 0;

  case WM_COMMAND:
    if (LOWORD(wParam) == 123)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (LOWORD(wParam) == 124)
    {
      DWORD style = GetWindowLong(hWnd, GWL_STYLE);
 
      if (style & WS_MAXIMIZE)
        ShowWindow(hWnd, SW_NORMAL);
      else
        ShowWindow(hWnd, SW_MAXIMIZE);
      /* DestroyWindow((HWND)lParam); */
    }
    else if (LOWORD(wParam) == 125)
    {
      DWORD state = SendMessage((HWND)lParam, BM_GETSTATE, 0, 0);
 
      is_select = !!(state & BST_CHECKED);
 
      if (is_select)
        SendMessage((HWND)lParam, BM_SETCHECK, BST_UNCHECKED, 0);
      else
        SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, 0);
      InvalidateRect(hWnd, NULL, FALSE);
    }
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 1;

  case WM_MOUSEMOVE:
    hDC = GetDC(hWnd);
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W, H);
    
    srand(30);
    for (i = 0; i < 200; i++)
    {
      SelectObject(hMemDC, GetStockObject(DC_PEN));
      SetDCPenColor(hMemDC, RGB(rand() % 255, rand() % 255, rand() % 255));

      DrawArrow2(hWnd, hMemDC, rand() % W, rand() % H, 30, W, H );
      DrawArrow(hWnd, hMemDC, rand() % W, rand() % H, 30, W, H );
    }
    ReleaseDC(hWnd, hDC);
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