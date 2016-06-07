/* FILE NAME: SPHERE.H
 * PROGRAMMER: AL5
 * DATE: 07.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include <math.h>
#include <time.h>

#include "SPHERE.H"

#define N 15
#define M 30

typedef struct 
{
  DOUBLE X, Y, Z;
} VEC;

VOID DrawSphere( HDC hDC, INT Xc, INT Yc, INT R )
{
  INT i, j, x, y, z, r = 2;  
  DOUBLE theta, phi, phase = clock() / 10000.0, M_PI = 3.14159265358979323846;
  static VEC G[N][M];
  
  /* Evaluate sphere ... */
  for (i = 0; i < N; i++)
  {
    theta = i * M_PI / (N - 1);
    for (j = 0; j < M; j++)
    {
      phi = j * 2.0 * M_PI / (M - 1) + phase;

      G[i][j].X = R * sin(theta) * cos(phi);
      G[i][j].X = R * sin(theta) * sin(phi);
      G[i][j].X = R * cos(theta);
      
    }
  }

  /* with points */
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < M; j++)
    {      
      x = G[i][j].X;
      y = G[i][j].Y;
      
      Ellipse(hDC, x - r, y - r, x + r, y + r); 
    }
  }

  /* horizontal */
  for (i = 0; i < N; i++)
  {
    theta = i * M_PI / (N - 1);
    x = Xc + R * sin(theta) * cos(phase);
    y = Yc - R * cos(theta);
    MoveToEx(hDC, x, y, NULL);

    for (j = 1; j < M; j++)
    {
      phi = j * 2 * M_PI / (M - 1) + phase;
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta);
      LineTo(hDC, x, y);
    }
  }     
  /* vertical */
  for (j = 0; j < M; j++)
  {
    phi = j * 2 * M_PI / (M - 1) + phase;
    x = Xc;
    y = Yc - R;
    
    MoveToEx(hDC, x, y, NULL);

    for (i = 1; i < N; i++)
    {
      theta = i * M_PI / (N - 1);
      x = Xc + R * sin(theta) * cos(phi);
      y = Yc - R * cos(theta); 
      LineTo(hDC, x, y);
    }
  }
}