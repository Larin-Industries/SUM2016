/* FILE NAME: RENDER.C
 * PROGRAMMER: AL5
 * DATE: 13.06.2016
 * PURPOSE: Render, just render...
 */

#include "ANIM.H"
#include <stdio.h>

MATR AL5_RndMatrWorld/* = MatrIdentity()*/,
     AL5_RndMatrView /* = MatrMulMatr(MatrIdentity, MatrTranslate(VecSet(-1, -1, 0)))*/,
     AL5_RndMatrProj;

FLT  AL5_RndProjDist  =   0.1,
     AL5_RndFarClip   =  100,
     AL5_RndProjSize  =   0.1;

VOID AL5_RndProj( VOID )
{
  FLT ratio_x = 1, ratio_y = 1;
  if (AL5_Anim.W >= AL5_Anim.H)
    ratio_x = (FLT)AL5_Anim.W / AL5_Anim.H;
  else
    ratio_y = (FLT)AL5_Anim.H / AL5_Anim.W;
  AL5_RndMatrProj = MatrFrustum(-ratio_x * AL5_RndProjSize / 2,
                                 ratio_x * AL5_RndProjSize / 2,
                                -ratio_y * AL5_RndProjSize / 2,
                                 ratio_y * AL5_RndProjSize / 2,
                                 AL5_RndProjDist, AL5_RndFarClip);
}


/* Primitive draw function.
 * ARGUMENTS:
 *   - primtive to draw:
 *       al5PRIM *Pr;
 * RETURNS: None.
 */
VOID AL5_RndPrimDraw( al5PRIM *Pr )
{
  INT loc;
  MATR M;

  /* Build transform matrix */
  M = MatrMulMatr(AL5_RndMatrWorld,
    MatrMulMatr(AL5_RndMatrView, AL5_RndMatrProj));
  glLoadMatrixf(M.A[0]);

  glUseProgram(AL5_RndPrg);

  /* Setup global variables */
  if ((loc = glGetUniformLocation(AL5_RndPrg, "MatrWorld")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AL5_RndMatrWorld.A[0]);
  if ((loc = glGetUniformLocation(AL5_RndPrg, "MatrView")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AL5_RndMatrView.A[0]);
  if ((loc = glGetUniformLocation(AL5_RndPrg, "MatrProj")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AL5_RndMatrProj.A[0]);
  if ((loc = glGetUniformLocation(AL5_RndPrg, "Time")) != -1)
    glUniform1f(loc, AL5_Anim.Time);


  /* Activete primitive vertex array */
  glBindVertexArray(Pr->VA);
  /* Activete primitive index buffer */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  /* Draw primitive */
  glDrawElements(GL_TRIANGLES, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
} /* End of 'AL5_RndPrimDraw' function */

/* Primitive free function.
 * ARGUMENTS:
 *   - primtive to free:
 *       al5PRIM *Pr;
 * RETURNS: None.
 */
VOID AL5_RndPrimFree( al5PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  glDeleteBuffers(1, &Pr->IBuf);
  memset(Pr, 0, sizeof(al5PRIM));
} /* End of 'AL5_RndPrimFree' function */

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
  INT NumOfV;
  INT NumOfI;
  CHAR Mtl[300];
  INT p;
  al5VERTEX *V;
  INT *I;

  memset(Pr, 0, sizeof(al5PRIM));

  F = fopen(FileName, "rb");
  if (F == NULL)
    return FALSE;

  /* File structure:
   *   4b Signature: "G3D\0"    CHAR Sign[4];
   *   4b NumOfPrimitives       INT NumOfPrimitives;
   *   300b material file name: CHAR MtlFile[300];
   *   repeated NumOfPrimitives times:
   *     4b INT NumOfV; - vertex count
   *     4b INT NumOfI; - index (triangles * 3) count
   *     300b material name: CHAR Mtl[300];
   *     repeat NumOfV times - vertices:
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
    fread(&NumOfV, 4, 1, F);
    fread(&NumOfI, 4, 1, F);
    fread(Mtl, 1, 300, F);

    /* Allocate memory for primitive */
    if ((V = malloc(sizeof(al5VERTEX) * NumOfV)) == NULL)
    {
      fclose(F);
      return FALSE;
    }
    if ((I = malloc(sizeof(INT) * NumOfI)) == NULL)
    {
      free(V);
      V = NULL;
      fclose(F);
      return FALSE;
    }
    Pr->NumOfI = NumOfI;
    fread(V, sizeof(al5VERTEX), NumOfV, F);
    fread(I, sizeof(INT), NumOfI, F);

    /* Create OpenGL buffers */
    glGenVertexArrays(1, &Pr->VA);
    glGenBuffers(1, &Pr->VBuf);
    glGenBuffers(1, &Pr->IBuf);

    /* Activate vertex array */
    glBindVertexArray(Pr->VA);
    /* Activate vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    /* Store vertex data */
    glBufferData(GL_ARRAY_BUFFER, sizeof(al5VERTEX) * NumOfV, V, GL_STATIC_DRAW);

    /* Setup data order */
    /*                    layout,
     *                      components count,
     *                          type
     *                                    should be normalize,
     *                                           vertex structure size in bytes (stride),
     *                                               offset in bytes to field start */
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(al5VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(al5VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(al5VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(al5VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

    /* Enable used attributes */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    /* Indices */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);

    /* Disable vertex array */
    glBindVertexArray(0);

    free(V);
    free(I);
    break;
  }
  fclose(F);
  return TRUE;
} /* End of 'AL5_RndPrimLoad' function */