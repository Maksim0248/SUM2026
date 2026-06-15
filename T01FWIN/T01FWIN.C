/* Eremeev Maksim, 09-3, 01.06.2026 */

#include <windows.h>

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

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt;
  SYSTEMTIME st;
  CHAR Buf[100];
  static BOOL is_select = FALSE;
  static INT W, H;

  switch (Msg)
  {

  case WM_CREATE:
    SetTimer(hWnd, 30, 500, NULL);
    return 0;

  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, TRUE);
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
 
    /* SelectObject(hDC, GetStockObject(DC_BRUSH)); */
    SetDCBrushColor(hDC, RGB(50, 50, 250));
    Rectangle(hDC, 100, 100, 300, 200);
    
    /* GDI */
 
    if (is_select)
      SelectObject(hDC, GetStockObject(NULL_BRUSH));
    else
      SelectObject(hDC, GetStockObject(WHITE_BRUSH));
    SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, RGB(10, 250, 0));
 
    Ellipse(hDC, 0, 0, W, H);
 
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
 
    SetDCPenColor(hDC, RGB(50, 200, 0));
    Ellipse(hDC, pt.x - 5, pt.y - 5, pt.x + 5, pt.y + 5);
 
    GetLocalTime(&st);
    TextOut(hDC, 130, 30, Buf, wsprintf(Buf, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond));
 
    EndPaint(hWnd, &ps);
    return 0;

  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure close window?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_DESTROY:
    KillTimer(hWnd, 30);
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}