/* FILE NAME: CONTROL.C
 * PROGRAMMER: AL5
 * DATE: 14.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "ANIM.H"

VOID AL5_RndPrimDraw( al5PRIM *Pr );

typedef struct
{
  al5UNIT;       
  VEC Pos;
}al5CONTROL;


static VOID AL5_UnitInit( al5CONTROL *Uni, al5ANIM *Ani )
{   
  Uni->Pos = VecSet(2, 2, 2);
} /* End of 'AL5_UnitInit' function */

static VOID AL5_UnitRender( al5CONTROL *Uni, al5ANIM *Ani )
{

}

static VOID AL5_UnitResponse( al5CONTROL *Uni, al5ANIM *Ani )
{
  DBL r;

  if (Ani->Keys['T'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->Keys['Y'])
    glPolygonMode(GL_FRONT, GL_LINE);
  if (Ani->Keys['U'])
    glPolygonMode(GL_BACK, GL_LINE);
  /*if (Ani->Keys[VK_SPACE])
    AL5_AnimAddUnit(AL5_UnitCreateBall()); */
  if (Ani->Keys['C'])
    AL5_AnimAddUnit(AL5_UnitCreateCube(Rnd0() * 50, Rnd0() * 50, Rnd0() * 50));
  if (Ani->KeysClick[VK_RETURN] && Ani->Keys[VK_MENU])
    AL5_FlipFullScreen(AL5_Anim.hWnd);
  /*if (Ani->KeysClick[VK_ESCAPE])
    AL5_AnimDoExit();*/
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  /* Uni->Pos.Y += Ani->JY * Ani->GlobalDeltaTime; */
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateX(10 * Ani->JY * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(10 * Ani->JX * Ani->GlobalDeltaTime));

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(10 * Ani->Mdx * Ani->GlobalDeltaTime));
    Uni->Pos = PointTransform(Uni->Pos, MatrRotateX(10 * Ani->Mdy * Ani->GlobalDeltaTime));
  }

  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(10 * Ani->Keys[VK_RIGHT] * Ani->GlobalDeltaTime));
  Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(-10 * Ani->Keys[VK_LEFT] * Ani->GlobalDeltaTime));

  r = VecLen(Uni->Pos);
  Uni->Pos = VecMulNum(Uni->Pos, (r + (-Ani->Mdz) * Ani->DeltaTime * 1.0) / r);

  AL5_RndMatrView = MatrView(Uni->Pos, VecSet(0, 0, 0), VecSet(0, 1, 0));
}

al5UNIT * AL5_UnitCreateControl( FLT x, FLT y, FLT z )
{
  al5CONTROL *Uni;

  if ((Uni = (al5CONTROL *)AL5_AnimUnitCreate(sizeof(al5CONTROL))) == NULL)
    return NULL;
  Uni->Pos = VecSet(x, y, z); 

  /* Setup unit methods */
  Uni->Init = (VOID *)AL5_UnitInit;
  Uni->Render = (VOID *)AL5_UnitRender;
  Uni->Response = (VOID *)AL5_UnitResponse;
  
  return (al5UNIT *)Uni;
} /* End of 'AL5_UnitCreateCONTROL' function */