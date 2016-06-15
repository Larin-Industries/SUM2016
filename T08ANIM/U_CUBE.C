/* FILE NAME: U_BALL.C
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: Ball unit sample.
 */

#include "ANIM.H"

VOID AL5_RndPrimDraw( al5PRIM *Pr );
typedef struct
{
  al5UNIT;
  VEC Pos;
  al5PRIM p;
}al5CUBE;

static VOID AL5_UnitInit( al5CUBE *Uni, al5ANIM *Ani )
{  
 
  AL5_RndPrimLoad(&Uni->p, "cow.g3d"); 
} /* End of 'AL5_UnitInit' function */

static VOID AL5_UnitRender( al5CUBE *Uni, al5ANIM *Ani )
{
  AL5_RndMatrWorld = MatrMulMatr(AL5_RndMatrWorld, MatrTranslate(Uni->Pos));
  AL5_RndPrimDraw(&Uni->p);
}

static VOID AL5_UnitClose( al5CUBE *Uni, al5ANIM *Ani )
{
  AL5_RndPrimFree(&Uni->p);
}


static VOID AL5_UnitResponse( al5CUBE *Uni, al5ANIM *Ani )
{
  
}

al5UNIT * AL5_UnitCreateCube( FLT x, FLT y, FLT z )
{
  al5CUBE *Uni;

  if ((Uni = (al5CUBE *)AL5_AnimUnitCreate( sizeof(al5CUBE) )) == NULL)
    return NULL;
  Uni->Pos = VecSet(x, y, z); 

  /* Setup unit methods */

  Uni->Init = (VOID *)AL5_UnitInit;
  Uni->Render = (VOID *)AL5_UnitRender;
  Uni->Response = (VOID *)AL5_UnitResponse;
  Uni->Close = (VOID *)AL5_UnitClose;
  return (al5UNIT *)Uni;
} /* End of 'AL5_UnitCreateBall' function */


/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       al5PRIM *Pr;
 * RETURNS: None.
 */