/* FILE NAME: U_BALL.C
 * PROGRAMMER: VG4
 * DATE: 10.06.2016
 * PURPOSE: Ball unit sample.
 */

#include "ANIM.H"

typedef struct
{
  al5UNIT;        /* Base unit fields */
  VEC Pos;        /* Cube position */
  al5PRIM Pr;     /*  */
  COLORREF Color; /* Unit color */
} al5UNIT_CUBE;

VEC CubeP[] =
{
  {-1, -1,  1},
  { 1, -1,  1},
  { 1, -1, -1},
  {-1, -1, -1},
  {-1,  1,  1},
  { 1,  1,  1},
  { 1,  1, -1},
  {-1,  1, -1}
};

INT CubeE[][2] =
{
  {0, 1}, {1, 2}, {2, 3}, {3, 0},
  {4, 5}, {5, 6}, {6, 7}, {7, 4},
  {0, 4}, {1, 5}, {2, 6}, {3, 7}
};

al5PRIM Cube =
{
  CubeP, sizeof(CubeP) / sizeof(CubeP[0]),
  CubeE, sizeof(CubeE) / sizeof(CubeE[0])
};

/* Unit cube initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_CUBE *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitInit( al5UNIT_CUBE *Uni, al5ANIM *Ani )
{
  Uni->Pos = VecSet(Rnd1(), Rnd1(), Rnd1());
  Uni->Color = RGB(255, 255, 255);
  AL5_RndPrimLoad(Uni->Pr, "COW.G3D");
} /* End of 'AL5_UnitInit' function */

/* Unit cube inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_CUBE *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitResponse( al5UNIT_CUBE *Uni, al5ANIM *Ani )
{
} /* End of 'AL5_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       al5UNIT_CUBE *Uni;
 *   - animation context:
 *       al5ANIM *Ani;
 * RETURNS: None.
 */
static VOID AL5_UnitRender( al5UNIT_CUBE *Uni, al5ANIM *Ani )
{
  SetDCBrushColor(Ani->hDC, Uni->Color);
  AL5_RndMatrWorld = MatrTranslate(Uni->Pos);
  AL5_RndPrimDraw(&Cube);
} /* End of 'AL5_UnitRender' function */

/* Unit ball creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (al5UNIT *) pointer to created unit.
 */
al5UNIT * AL5_UnitCreateCube( VOID )
{
  al5UNIT_CUBE *Uni;

  if ((Uni = (al5UNIT_CUBE *)AL5_AnimUnitCreate(sizeof(al5UNIT_CUBE))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AL5_UnitInit;
  Uni->Response = (VOID *)AL5_UnitResponse;
  Uni->Render = (VOID *)AL5_UnitRender;

  return (al5UNIT *)Uni;
} /* End of 'AL5_UnitCreateCube' function */

/* END OF 'U_CUBE.C' FILE */