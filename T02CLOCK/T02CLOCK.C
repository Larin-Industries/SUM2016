/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: AL5
 * DATE: 02.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My Window Class"

#define M_PI 3.14159265358979323846

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
  DOUBLE alpha, beta, gamma;
  CHAR Data[50];
  HDC hDC;
  HPEN hPen, hOldPen;
  PAINTSTRUCT ps;
  SYSTEMTIME lt;
  static INT w, h;
  static BITMAP Bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    hBmLogo = LoadImage(NULL, "Clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(Bm), &Bm);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    SelectObject(hMemDCLogo, hBmLogo);
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
    hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hMemDC, hPen);
    GetLocalTime(&lt);
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);
    DeleteObject(hPen);
    StretchBlt(hMemDC, w / 2 - h / 2, 0, h, h, hMemDCLogo, 0, 0, Bm.bmWidth, Bm.bmHeight, SRCCOPY);

    alpha = (lt.wSecond * M_PI + lt.wMilliseconds / 1000) / 30.0;
    beta = (lt.wMinute * M_PI + lt.wMinute / 60) / 30.0;
    gamma = lt.wHour * M_PI / 6.0;
    /* Seconds */
    hPen = CreatePen(PS_SOLID, 2, RGB(10, 10, 10));
    hOldPen = SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, w / 2, h / 2, NULL);     
    LineTo(hMemDC, (INT)(w / 2 + sin(alpha) * h / 2), 
      (INT)(h / 2 - cos(alpha) * h / 2));
    SelectObject(hMemDC, hOldPen);
    DeleteObject(hPen);
    /* Minutes */
    hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 50));
    hOldPen = SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, (INT)(w / 2 + sin(beta) * h / 3), 
      (INT)(h / 2 - cos(beta) * h / 3));
    SelectObject(hMemDC, hOldPen);
    DeleteObject(hPen);
    /* Hours */
    hPen = CreatePen(PS_SOLID, 10, RGB(100, 0, 0));
    hOldPen = SelectObject(hMemDC, hPen);
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, (INT)(w / 2 + sin(gamma) * h / 4), 
      (INT)(h / 2 - cos(gamma) * h / 4));
    SelectObject(hMemDC, hOldPen);
    DeleteObject(hPen);

    hPen = CreatePen(PS_SOLID, 10, RGB(0, 0, 0));
    hOldPen = SelectObject(hMemDC, hPen);
    Ellipse(hMemDC, w / 2 - 5, h / 2 + 5, w / 2 + 5, h / 2 - 5);

    TextOut(hMemDC, 20, h - 50, Data, sprintf(Data, "%02i.%02i.%04i;"
      "  Time: %02i:%02i:%02i", lt.wDay, lt.wMonth, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond));

    SetBkMode(hMemDC, TRANSPARENT);
    InvalidateRect(hWnd, NULL, FALSE);

    SelectObject(hMemDC, hOldPen);
    DeleteObject(hPen);
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
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWinFunc' function */