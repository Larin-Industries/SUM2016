/* FILE NAME: SPHERE.H
 * PROGRAMMER: AL5
 * DATE: 07.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include <time.h>

#include "SPHERE.H"

#define N 500
#define M 1000

typedef struct
{
  HBITMAP hBmGlobe;
  DWORD *Bits;
  INT W, H;
} IMG;

VOID DrawSphere( HDC hDC, INT Xc, INT Yc, INT R )
{
  INT i, j, x, y, z, r = 2, img_x, img_y;
  COLORREF c;
  BYTE r1, g, b;
  DBL theta, phi, phase = clock() / 1000.0;
  static VEC G[N][M];
  HPEN hPen, hOldPen;

  hPen = CreatePen(PS_SOLID, 4, RGB(150, 150, 200));
  hOldPen = SelectObject(hDC, hPen);

  /* with points */
  
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < M; j++)
    {      
      x = Xc + G[i][j].X;
      y = Yc - G[i][j].Z;
      
      Ellipse(hDC, x - r, y - r, x + r, y + r); 
    }
  }
  
  /* horizontal */
  
  for (i = 0; i < N; i++)
  {
    MoveToEx(hDC, Xc + G[i][0].X, Yc - G[i][0].Z, NULL);
    for (j = 1; j < M; j++)    
      LineTo(hDC, Xc + G[i][j].X, Yc - G[i][j].Z);
  } 
 
  /* vertical */
 
  for (j = 0; j < M; j++)
  { 
    MoveToEx(hDC, Xc, Yc - R, NULL);
    for (i = 1; i < N; i++)
      LineTo(hDC, Xc + G[i][j].X, Yc - G[i][j].Z);
  }

  SelectObject(hDC, hOldPen);
  DeleteObject(hPen);
}