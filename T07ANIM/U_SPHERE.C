/* FILE NAME: U_SPHERE.C
 * PROGRAMMER: AL5
 * DATE: 12.06.2016
 * PURPOSE: Sphere unit sample.
 */

#include <time.h>

#include "ANIM.H"

#define N 10
#define M 20

/* Sphere unit representation type */
typedef struct
{
  al5UNIT;              /* Base unit fields */
  VEC Pos;              /* Sphere position */
  VEC Shift;            /* Sphere shift position */
  INT R;                /* Radius of sphere */
  COLORREF Color;       /* Color */
} al5UNIT_SPHERE;

/* Unit sphere initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */

/* Position of the sphere */
static INT xPos = 200, yPos = 200;

static VOID AL5_UnitInit( al5UNIT_SPHERE *Uni, al5ANIM *Ani )
{
  Uni->Pos = VecSet(xPos, yPos, 0);
  Uni->Color = RGB(127 * (sin(clock() / CLOCKS_PER_SEC) + 1), 0, 0);
  Uni->R = 200;
} /* End of 'AL5_UnitInit' function */

/* Unit sphere inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitResponse(al5UNIT_SPHERE *Uni, al5ANIM *Ani)
{
  INT newX = 0, newY = 0;

  if (Ani->Keys[37] && xPos - 20 > 0)
    newX = -20;
  if (Ani->Keys[39] && xPos + 20 < Ani->W)
    newX = 20;
  if (Ani->Keys[40] && yPos + 20 < Ani->H)
    newY = 20;
  if (Ani->Keys[38] && yPos - 20 > 0)
    newY = -20;

  xPos += newX;
  yPos += newY;
    
  Uni->Shift = VecSet(xPos, yPos, 0);
} /* End of 'AL5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitRender( al5UNIT_SPHERE *Uni, al5ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);
  INT i, j, x, y, z, r = 2;
  BYTE r1, g, b;
  DBL theta, phi, phase = clock() / 1000.0;
  static VEC G[N][M];
  HPEN hPen, hOldPen;

  hPen = CreatePen(PS_SOLID, 4, Uni->Color);
  hOldPen = SelectObject(Ani->hDC, hPen);

  /* Evaluate sphere ... */
  for (i = 0; i < N; i++)
  {
    theta = i * M_PI / (N - 1);
    for (j = 0; j < M; j++)
    {
      phi = j * 2.0 * M_PI / (M - 1) + phase;

      G[i][j].X = Uni->R * sin(theta) * cos(phi);
      G[i][j].Y = Uni->R * sin(theta) * sin(phi);
      G[i][j].Z = Uni->R * cos(theta);
    }
  }

  /* Drawing sphere with points */
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      x = p.X + G[i][j].X;
      y = p.Y - G[i][j].Z;

      Ellipse(Ani->hDC, x - r, y - r, x + r, y + r);
    }

  /* Drawing horizontal lines */
  for (i = 0; i < N; i++)
  {
    MoveToEx(Ani->hDC, p.X + G[i][0].X, p.Y - G[i][0].Z, NULL);
    for (j = 1; j < M; j++)
      LineTo(Ani->hDC, p.X + G[i][j].X, p.Y - G[i][j].Z);
  }

  /* Drawing vertical lines */
  for (j = 0; j < M; j++)
  {
    MoveToEx(Ani->hDC, p.X, p.Y - Uni->R, NULL);
    for (i = 1; i < N; i++)
      LineTo(Ani->hDC, p.X + G[i][j].X, p.Y - G[i][j].Z);
  }

  SelectObject(Ani->hDC, hOldPen);
  DeleteObject(hPen);
} /* End of 'AL5_UnitRender' function */

/* Unit sphere creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (al5UNIT *) pointer to created unit.
 */
al5UNIT * AL5_UnitCreateSphere(VOID)
{
  al5UNIT_SPHERE *Uni;

  if ((Uni = (al5UNIT_SPHERE *)AL5_AnimUnitCreate(sizeof(al5UNIT_SPHERE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AL5_UnitInit;
  Uni->Response = (VOID *)AL5_UnitResponse;
  Uni->Render = (VOID *)AL5_UnitRender;

  return (al5UNIT *)Uni;
} /* End of 'AL5_UnitCreateBall' function */

/* END OF 'U_SPHERE.C' FILE */