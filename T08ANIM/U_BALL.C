/* FILE NAME: U_BALL.C
 * PROGRAMMER: VG4
 * DATE: 10.06.2016
 * PURPOSE: Ball unit sample.
 */

#include <time.h>

#include "ANIM.H"

/* Ball unit representation type */
typedef struct
{
  al5UNIT; /* Base unit fields */
  VEC Pos; /* Ball position */
  VEC Shift; /* Ball shift position */
  DBL TimerShift; /* Timer shift phase value*/
  DBL TimerSpeed; /* Timer speed value*/
  COLORREF Color; /* Color */
} al5UNIT_BALL;

/* Unit ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitInit( al5UNIT_BALL *Uni, al5ANIM *Ani )
{
  Uni->Pos = VecSet(Ani->W / 2, Ani->H / 2, 0);
  Uni->TimerShift = 9;
  Uni->TimerSpeed = 8;
  Uni->Color = RGB(55, 255, 5);
} /* End of 'AL5_UnitInit' function */

/* Unit ball inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitResponse( al5UNIT_BALL *Uni, al5ANIM *Ani )
{
  Uni->Shift = VecSet(30 * Ani->JX, 30 * Ani->JY, 0);
} /* End of 'AL5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitRender( al5UNIT_BALL *Uni, al5ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, p.X - 10, p.Y - 10, p.X + 10, p.Y + 10);
} /* End of 'AL5_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (al5UNIT *) pointer to created unit.
 */
al5UNIT * AL5_UnitCreateBall( VOID )
{
  al5UNIT_BALL *Uni;

  if ((Uni = (al5UNIT_BALL *)AL5_AnimUnitCreate(sizeof(al5UNIT_BALL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AL5_UnitInit;
  Uni->Response = (VOID *)AL5_UnitResponse;
  Uni->Render = (VOID *)AL5_UnitRender;

  return (al5UNIT *)Uni;
} /* End of 'AL5_UnitCreateBall' function */

/* END OF 'U_BALL.C' FILE */