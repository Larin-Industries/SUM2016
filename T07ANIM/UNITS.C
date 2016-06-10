/* FILE NAME: UNITS.C
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "ANIM.H"

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitInit( al5UNIT *Uni, al5ANIM *Ani )
{
} /* End of 'AL5_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitClose( al5UNIT *Uni, al5ANIM *Ani )
{
} /* End of 'AL5_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitResponse( al5UNIT *Uni, al5ANIM *Ani )
{
} /* End of 'AL5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitRender( al5UNIT *Uni, al5ANIM *Ani )
{
} /* End of 'AL5_UnitRender' function */

/* Unit creation function.
 * ARGUMENTS:
 *   - unit structure size in bytes:
 *       INT Size;
 * RETURNS:
 *   (al5UNIT *) pointer to created unit.
 */
al5UNIT * AL5_AnimUnitCreate( INT Size )
{
  al5UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(al5UNIT) ||
      (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = AL5_UnitInit;
  Uni->Close = AL5_UnitClose;
  Uni->Response = AL5_UnitResponse;
  Uni->Render = AL5_UnitRender;
  return Uni;
} /* End of 'AL5_AnimUnitCreate' function */
