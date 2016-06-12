/* FILE NAME: U_BALL_CONT.C
 * PROGRAMMER: AL5
 * DATE: 11.06.2016
 * PURPOSE: Control ball unit sample.
 */

#include "ANIM.H"

/* Ball unit representation type */
typedef struct
{
  al5UNIT;        /* Base unit fields */
  VEC Pos;        /* Ball position */
  VEC Shift;      /* Ball shift position */
  DBL TimerShift; /* Timer shift phase value*/
  DBL TimerSpeed; /* Timer speed value*/
  COLORREF Color; /* Color */
} al5UNIT_BALL_CONT;

/* Unit ball initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static INT bX = 0, bY = 0;

static VOID AL5_UnitInit( al5UNIT_BALL_CONT *Uni, al5ANIM *Ani )
{
  Uni->Pos = VecSet(bX, bY, 0);
  Uni->Color = RGB(100, 100, 100);
} /* End of 'AL5_UnitInit' function */

/* Unit ball inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitResponse( al5UNIT_BALL_CONT *Uni, al5ANIM *Ani )
{
  if ((bX + 30.0 * Ani->JX < Ani->W || bX - 30.0 * Ani->JX > 0) &&
      (bY + 30.0 * Ani->JY < Ani->H || bY - 30.0 * Ani->JY > 0))
  {
    bX += 30.0 * Ani->JX;
    bY += 30.0 * Ani->JY;
  }
  
  Uni->Shift = VecSet(Ani->Mx, Ani->My, 0);

  if (Ani->JBut[1])
    bX = Ani->W / 2, bY = Ani->H / 2;
} /* End of 'AL5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_BALL *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitRender( al5UNIT_BALL_CONT *Uni, al5ANIM *Ani )
{
  VEC p = VecAddVec(Uni->Pos, Uni->Shift);
  
  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, p.X - 20, p.Y - 20, p.X + 20, p.Y + 20);
} /* End of 'AL5_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (al5UNIT *) pointer to created unit.
 */
al5UNIT * AL5_UnitCreateBallCont( VOID )
{
  al5UNIT_BALL_CONT *Uni;

  if ((Uni = (al5UNIT_BALL_CONT *)AL5_AnimUnitCreate(sizeof(al5UNIT_BALL_CONT))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AL5_UnitInit;
  Uni->Response = (VOID *)AL5_UnitResponse;
  Uni->Render = (VOID *)AL5_UnitRender;

  return (al5UNIT *)Uni;
} /* End of 'AL5_UnitCreateBall' function */

/* END OF 'U_BALL_CONT.C' FILE */