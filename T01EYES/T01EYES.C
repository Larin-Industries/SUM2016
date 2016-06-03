/* FILE NAME: T01EYES.C
 * PROGRAMMER: AL5
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include <math.h>
#include <windows.h>

#define WND_CLASS_NAME "Larin Industries"

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawEye( HWND hWnd, HDC hDc, INT X, INT Y, INT R, INT R1 );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  HWND hWnd;
  WNDCLASS wc;
  MSG msg;

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
    MessageBox(NULL, "Error", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME,
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return msg.wParam;
} /* End of 'WinMain' function */

LRESULT CALLBACK MyWinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  INT i, j;
  static INT w, h, moveX[4] = {52, 90, 120, 184}, moveY[4] = {184, 120, 90, 52},
    plusX[4] = {15, -20, 30, 10}, plusY[4] = {15, 20, -30, -10}, R = 51, R1;
  static CHAR plusR = 1;
  static HBITMAP hBm;
  static HDC hMemDC;

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
  case WM_TIMER:
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    hDC = BeginPaint(hWnd, &ps);

    for (i = 0; i < 4; i++)
    {
      if (moveX[i] >= w - R - plusR || moveX[i] <= R - plusR)
        plusX[i] = -plusX[i];
      if (moveY[i] >= h - R - plusR || moveY[i] <= R - plusR)
        plusY[i] = -plusY[i];
      moveX[i] += plusX[i];
      moveY[i] += plusY[i];

      DrawEye(hWnd, hMemDC, moveX[i], moveY[i], R, R1);
    }

    if (R >= 200 || R <= 50)
      plusR = -plusR;
      
    R += plusR;
    R1 = R / 3;


    /*
      Many little eyes:
        
    for (i = 25; i < w; i += 50)
      for (j = 25; j < h; j += 50)
        DrawEye(hWnd, hMemDC, i, j, 24, 8);
     */
    /*
      4 eyes:
    
    DrawEye(hWnd, hMemDC, w / 4, h / 2, 300, 50);
    DrawEye(hWnd, hMemDC, 3 * w / 4, h / 2, 300, 50);
    DrawEye(hWnd, hMemDC, w / 2 - 50, h - 50, 40, 10);
    DrawEye(hWnd, hMemDC, w / 2 + 50, h - 50, 40, 10);
     */
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

VOID DrawEye( HWND hWnd, HDC hDC, INT X, INT Y, INT R, INT R1 )
{
  POINT pt;
  INT dx, dy;
  DOUBLE alpha;
  
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(200, 200, 200));  
  Ellipse(hDC, X - R, Y - R, X + R, Y + R);
  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  dx = pt.x - X;
  dy = pt.y - Y;

  alpha = (R - R1) / sqrt(dx * dx + dy * dy);
  if (alpha < 1)
  {
    dx *= alpha;
    dy *= alpha;
  }

  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));  
  Ellipse(hDC, dx - R1 + X, Y + dy - R1, X + dx + R1, Y + dy + R1);
} /* End of 'DrawEye' function */