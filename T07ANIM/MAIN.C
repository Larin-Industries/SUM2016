/* FILE NAME: T07ANIM.C
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "ANIM.H"

#define AL5_WND_CLASS_NAME "Larin Industries"

LRESULT CALLBACK AL5_WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  HWND hWnd;
  WNDCLASS wc;
  MSG msg;

  SetDbgMemHooks();

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hInstance = hInstance;
  wc.lpszClassName = AL5_WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = AL5_WinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(AL5_WND_CLASS_NAME,
    "30!",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  AL5_AnimAddUnit(AL5_UnitCreateBallCont());
  AL5_AnimAddUnit(AL5_UnitCreateSphere());

  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 0;
} /* End of 'WinMain' function */

LRESULT CALLBACK AL5_WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  static HBITMAP hBm;
  static HDC hMemDC;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    AL5_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    AL5_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_MOUSEMOVE:
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_MOUSEWHEEL:
    AL5_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_KEYDOWN:
//    if (VK_SPACE)
//      AL5_AnimAddUnit(AL5_UnitCreateBall());
    return 0;
  case WM_TIMER:
    AL5_AnimRender();
    return 0;
  case WM_ERASEBKGND:
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    AL5_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:    
    AL5_AnimClose();
    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWinFunc' function */