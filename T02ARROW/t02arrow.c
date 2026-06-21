/* Eremeev Maksim, 09-3, 01.06.2026 */

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
  INT x, y;
  DOUBLE len;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  len = hypot(pt.x - Xc, pt.y - Yc);
  x = Xc + (pt.x - Xc)* L / len;
  y = Yc + (pt.y - Yc)* L / len;
  MoveToEx(hDC, Xc, Yc, NULL);
  LineTo(hDC, x, y);
}

void DrawArrow2( HWND hWnd, HDC hDC, INT Xc, INT Yc, INT W, INT H )
{
  POINT pts[7] = {{-4, -10}, {4, -10}, {4, 6}, {10, 6}, {0, 14}, {-10, 6}, {-4, 6}};
  POINT pts_res[sizeof(pts) / sizeof(pts[0])]; 
  int N = sizeof(pts) / sizeof(pts[0]);
  POINT pt;
  INT i;
  DOUBLE len;


  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);
  len = hypot(pt.x - Xc, pt.y - Yc);
  for (i = 0; i < N; i++)
  {
    pts_res[i].x = Xc + pts[i].x * (pt.y - Yc)/len + pts[i].y * (pt.x - Xc)/len;
    pts_res[i].y = Yc + pts[i].y * (pt.y - Yc)/len - pts[i].x * (pt.x - Xc)/len;
  }
  Polygon(hDC, pts_res, N);
}



LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  static BOOL is_select = FALSE;
  static INT W, H;
  INT i;
  static HDC hMemDC; /*контекст растового изображения в памяти*/
  static HBITMAP hBm; /*растовое изображение(буффер) в памяти*/
  PAINTSTRUCT ps;


  switch (Msg)
  {

  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);/*копия контекста экрана*/
    ReleaseDC(hWnd, hDC);

    SetTimer(hWnd, 30, 500, NULL);
    return 0;

  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);
    SelectObject(hMemDC, hBm);
    ReleaseDC(hWnd, hDC);
    SendMessage(hWnd, WM_TIMER, 30, 0);
    return 0;

  case WM_TIMER:
    hDC = GetDC(hWnd);
    Rectangle(hMemDC, 0, 0, W, H);
    SetDCBrushColor(hDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, W, H);
    srand(30);
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    for (i = 0; i < 200; i++)
    {
      SetDCBrushColor(hMemDC, RGB(rand() % 255, rand() % 255, rand() % 255));
      DrawArrow2(hWnd, hMemDC, rand() % W, rand() % H, W, H );

      DrawArrow(hWnd, hMemDC, rand() % W, rand() % H, 30, W, H );
    }
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    
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

  case WM_MOUSEMOVE:
    SendMessage(hWnd, WM_TIMER, 30, 0);
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