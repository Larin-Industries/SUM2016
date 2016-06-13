/* FILE NAME: RENDER.C
 * PROGRAMMER: AL5
 * DATE: 13.06.2016
 * PURPOSE: Render, just render...
 */

#include "ANIM.H"

al5ANIM AL5_Anim;

MATR
  AL5_RndMatrWorld,
  AL5_RndMatrView,
  AL5_RndMatrProj;

DBL
  AL5_RndProjDist = 5,
  AL5_RndFarClip = 50,
  AL5_RndProjSize = 10;

MATR MatrFrustum( DBL l, DBL r, DBL b, DBL t, DBL n, DBL f )
{
  MATR M = 
  {
    {
      {2 * n / (r - l)  , 0                , 0                   , 0},
      {0                , 2 * n / (t - b)  , 0                   , 0},
      {(r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n)  ,-1},
      {0                , 0                , -2 * n * f / (f - n), 0}
    }
  };

  return M;
}

MATR MatrView( VEC Loc, VEC Add, VEC Up1 )
{
  VEC
    Dir = VecNormalize(VecSubVec(Add, Loc)),
    Right = VecNormalize(VecCrossVec(Dir, Up1)),
    Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m = 
  {
    {
      {Right.X               , Up.X                , - Dir.X            , 0},
      {Right.Y               , Up.Y                , - Dir.Y            , 0},
      {Right.Z               , Up.Z                , - Dir.Z            , 0},
      {-VecDotVec(Loc, Right), - VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
}

VOID AL5_RndSetProj( VOID )
{
  DBL ratio_x = 1, ratio_y = 1;

  if (AL5_Anim.W >= AL5_Anim.H)
    ratio_x = (DBL)AL5_Anim.W / AL5_Anim.H;
  else
    ratio_y = (DBL)AL5_Anim.H / AL5_Anim.W;

  AL5_RndMatrProj = MatrFrustum(-ratio_x * AL5_RndProjSize / 2,
                                 ratio_x * AL5_RndProjSize / 2,
                                -ratio_y * AL5_RndProjSize / 2,
                                 ratio_y * AL5_RndProjSize / 2,
                                 AL5_RndProjDist, AL5_RndFarClip);
} /* End of 'AL5_RndSetProj' function */

/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       al5PRIM *Pr;
 * RETURNS: None.
 */
VOID AL5_RndPrimDraw( al5PRIM *Pr )
{
  MATR M;
  POINTS *pts;
  INT i;

  /* Build transform matrix */
  M = MatrMulMatr(AL5_RndMatrWorld, MatrMulMatr(AL5_RndMatrView, AL5_RndMatrProj));
  /* Transform all points */
  pts = malloc(sizeof(POINT) * Pr->NumOfP);
  if (pts == NULL)
    return;
  for (i = 0; i < Pr->NumOfP; i++)
  {
    VEC p = PointTransform(Pr->P[i], M);
    pts[i].x = (p.X + 1) * AL5_Anim.W / 2;
    pts[i].y = (p.Y + 1) * AL5_Anim.H / 2;
  }

  /* Draw all lines */
  for (i = 0; i < Pr->NumOfE; i++)
  {
    INT n0 = Pr->Edges[i][0], n1 = Pr->Edges[i][1];
    MoveToEx(AL5_Anim.hDC, pts[n0].x, pts[n0].y, NULL);
    LineTo(AL5_Anim.hDC, pts[n1].x, pts[n1].y);
  }
  free(pts);
} /* End of 'AL5_RndPrimDraw' function */

/* END OF 'RENDER.C' FILE */