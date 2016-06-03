/* FILE NAME: T03POLE.C
 * PROGRAMMER: AL5
 * DATE: 03.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include <time.h>
#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "Larin Industries"

#define M_PI 3.14159265358979323846

VOID PoleDraw( HWND hWnd, HDC hDC, INT X, INT Y, INT R, INT H );
/* Forward references */
LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* Register window class */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = MyWinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Create window */
  hWnd = CreateWindow(WND_CLASS_NAME,
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);
  if (hWnd == NULL)
  {
    MessageBox(NULL, "Create window erroe", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Show window */
  ShowWindow(hWnd, CmdShow);
  UpdateWindow(hWnd);

  /* Run message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT i, j;
  DOUBLE Need;
  HDC hDC;
  PAINTSTRUCT ps;
  static HBITMAP hBm;
  static HDC hMemDC;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    hDC = BeginPaint(hWnd, &ps);
    srand(255);
//    Need = (sin((DOUBLE)clock() / CLOCKS_PER_SEC) + 1.1);
    for (i = 0; i < w; i += 30)
      for (j = 0; j < h; j += 10)
        PoleDraw(hWnd, hMemDC, i, j, 90, 200);
    EndPaint(hWnd, &ps);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWinFunc' function */

VOID PoleDraw( HWND hWnd, HDC hDC, INT X, INT Y, INT W, INT H )
{
  POINT ptsUp[] = 
  {
    {0, W / 2}, {H / 2, 0}, {0, -W / 2}
  };
  POINT ptsUpSht[sizeof(ptsUp) / sizeof(ptsUp[0])];
  POINT ptsDown[] = 
  {
    {0, -W / 2}, {-H / 2, 0}, {0, W / 2}
  };
  POINT ptsDownSht[sizeof(ptsUp) / sizeof(ptsUp[0])];
  INT i;
  DOUBLE sinAlpha, cosAlpha;
  HPEN hPen;
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  sinAlpha = (pt.y - Y) / (sqrt((pt.y - Y) * (pt.y - Y) + (pt.x - X) * (pt.x - X)));
  cosAlpha = (pt.x - X) / (sqrt((pt.y - Y) * (pt.y - Y) + (pt.x - X) * (pt.x - X)));

  for (i = 0; i < sizeof(ptsUp) / sizeof(ptsUp[0]); i++)
  {
    ptsUpSht[i].x = X + ptsUp[i].x * cosAlpha - ptsUp[i].y * sinAlpha;
    ptsUpSht[i].y = Y + ptsUp[i].x * sinAlpha + ptsUp[i].y * cosAlpha;
    ptsDownSht[i].x = X + ptsDown[i].x * cosAlpha - ptsDown[i].y * sinAlpha;
    ptsDownSht[i].y = Y + ptsDown[i].x * sinAlpha + ptsDown[i].y * cosAlpha;
  }
  /*
  MoveToEx(hDC, X, Y, NULL);
  LineTo(hDC, pt.x, pt.y);
  */
  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(255, 0, 0));
//  SetDCBrushColor(hDC, RGB(rand() % 255, rand() % 255, rand() % 255));
  SetDCBrushColor(hDC, RGB(255, 0, 0));

  Polygon(hDC, ptsUpSht, 3);

  SetDCPenColor(hDC, RGB(0, 0, 255));
//  SetDCBrushColor(hDC, RGB(rand() % 255, rand() % 255, rand() % 255));
  SetDCBrushColor(hDC, RGB(0, 0, 255));

  Polygon(hDC, ptsDownSht, 3);

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
}