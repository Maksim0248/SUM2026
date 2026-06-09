/* Eremeev Maksim, 09-3, 06.06.2026 */

#include <windows.h>
#include <math.h>
#include "globe.h"
#include "mth.h"

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
  PAINTSTRUCT ps;
  static SYSTEMTIME st;
  static INT W, H;
  MINMAXINFO *minmax;

  static int Xc, Yc, lenm = 200, lenh = 100, x, y;

  /*Descriptors*/
  static HDC hDC;/*контекст окна*/

  static HDC hMemDC; /*контекст растового изображения в памяти*/
  static HBITMAP hBm; /*растовое изображение(буффер) в памяти*/

  
  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);/*копия контекста экрана*/
    SetTimer(hWnd, 30, 10, NULL);
    GLB_Init(0.3);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    return 0;

  case WM_SIZE:/* идет сразу после create */
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    Xc = W / 2 - 2;
    Yc = H / 2 + 5;
    GLB_Resize(W, H);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, W, H);/*функция создает картинку в памяти*/
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 30, 0);

    ReleaseDC(hWnd, hDC);

    return 0;
  case WM_TIMER:
    hDC = GetDC(hWnd);
    Rectangle(hMemDC, 0, 0, W, H);
    GLB_Draw(hMemDC);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_DESTROY, 0, 0);
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
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure close window?", "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_DESTROY:
    DeleteObject(hBm);
        KillTimer(hWnd, 30);
    PostMessage(NULL, WM_QUIT, 30, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}