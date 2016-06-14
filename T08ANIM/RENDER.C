/* FILE NAME: RENDER.C
 * PROGRAMMER: AL5
 * DATE: 13.06.2016
 * PURPOSE: Render, just render...
 */

#include <stdio.h>

#include "ANIM.H"

al5ANIM AL5_Anim;

MATR
  AL5_RndMatrWorld,
  AL5_RndMatrView,
  AL5_RndMatrProj;

DBL
  AL5_RndProjDist = 0.1,
  AL5_RndFarClip = 90,
  AL5_RndProjSize = 0.1;

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
  INT i;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(AL5_RndMatrWorld,
    MatrMulMatr(AL5_RndMatrView, AL5_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  /* Draw all lines */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI; i++)
  {
    glColor3fv(&Pr->V[Pr->I[i]].C.X);
    glVertex3fv(&Pr->V[Pr->I[i]].P.X);
  }
  glEnd();
} /* End of 'AL5_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       al5PRIM *Pr;
 * RETURNS: None.
 */
VOID AL5_RndPrimFree( al5PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  if (Pr->I != NULL)
    free(Pr->I);
  memset(Pr, 0, sizeof(al5PRIM));
} /* End of 'AL5_RndPrimFree' function */

VEC4 Vec4Set( DBL X, DBL Y, DBL Z, DBL W )
{
  VEC4 p;

  p.X = X;
  p.Y = Y;
  p.Z = Z;
  p.W = W;

  return p;
}

/* Load primitive from '*.g3d' file function.
 * ARGUMENTS:
 *   - primitive structure pointer:
 *       al5PRIM *Pr;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE is success, FALSE otherwise.
 */
BOOL AL5_RndPrimLoad( al5PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  DWORD Sign;
  INT NumOfPrimitives;
  CHAR MtlFile[300];
  INT NumOfP;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;

  memset(Pr, 0, sizeof(al5PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfP; - vertex count
   *     4b INT NumOfI; - index (triangles * 3) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfP times - vertices:
   *         !!! float point -> FLT
   *       typedef struct
   *       {
   *         VEC  P;  - Vertex position
   *         VEC2 T;  - Vertex texture coordinates
   *         VEC  N;  - Normal at vertex
   *         VEC4 C;  - Vertex color
   *       } VERTEX;
   *     repeat (NumOfF / 3) times - facets (triangles):
   *       INT N0, N1, N2; - for every triangle (N* - vertex number)
   */
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }
  fread(&NumOfPrimitives, 4, 1, F);
  fread(MtlFile, 1, 300, F);
  for (p = 0; p < NumOfPrimitives; p++)
  {
    /* Read primitive info */
    fread(&NumOfP, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((Pr->V = malloc(sizeof(al5VERTEX) * NumOfP)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((Pr->I = malloc(sizeof(INT) * NumOfI)) == NULL)
    {
      free(Pr->V);
      Pr->V = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfV = NumOfP;
    Pr->NumOfI = NumOfI;
    fread(Pr->V, sizeof(al5VERTEX), NumOfP, F);
    fread(Pr->I, sizeof(INT), NumOfI, F);
    if (Pr->NumOfV > 0)
    {
      INT i;

      for (i = 0; i < Pr->NumOfV; i++)
        Pr->V[i].C = Vec4Set(Pr->V[i].N.X / 2 + 0.5,
                             Pr->V[i].N.Y / 2 + 0.5,
                             Pr->V[i].N.Z / 2 + 0.5, 1);
    }
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'AL5_RndPrimLoad' function */


/* END OF 'RENDER.C' FILE */