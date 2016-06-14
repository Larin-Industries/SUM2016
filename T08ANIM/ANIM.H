/* FILE NAME: ANIM.H
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "DEF.H"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

#define AL5_MAX_UNITS 1000000

extern AL5_MouseWheel;

typedef struct tagal5UNIT al5UNIT;

typedef struct
{
  HWND hWnd; 
  HDC hDC;                       /* Drawing window context */
  HGLRC hGLRC;                   /* Rendering context */
  INT W, H;  
  HBITMAP hFrame;
  al5UNIT *Units[AL5_MAX_UNITS];
  INT NumOfUnits;                  /* Number of units */
  BYTE JBut[32];                   /* Joystick buttons */
  DBL JX, JY, JZ, JR;              /* Joystick axes */
  INT JPov;                        /* Point of viev */
  INT Mx, My, Mz,                  /* Mouse coordinates */
    Mdx, Mdy, Mdz,
    MOldx, MOldy, MOldz;
  BYTE Keys[256], KeysOld[256],    /* Keyboard keys */
    KeyClick[256];
} al5ANIM;


struct tagal5UNIT
{
  /* Unit initialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       al5UNIT *Uni;
   *   - animation context:
   *       al5ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Init)( al5UNIT *Uni, al5ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       al5UNIT *Uni;
   *   - animation context:
   *       al5ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Close)( al5UNIT *Uni, al5ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       al5UNIT *Uni;
   *   - animation context:
   *       al5ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Response)( al5UNIT *Uni, al5ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       al5UNIT *Uni;
   *   - animation context:
   *       al5ANIM *Ani;
   * RETURNS: None.
   */
  VOID (*Render)( al5UNIT *Uni, al5ANIM *Ani );
};

VOID AL5_AnimAddUnit( al5UNIT *Uni );
VOID AL5_AnimInit( HWND hWnd );
VOID AL5_AnimResize( INT W, INT H );
VOID AL5_AnimCopyFrame( VOID );
VOID AL5_AnimRender( VOID );
VOID AL5_AnimClose( VOID );
al5UNIT * AL5_AnimUnitCreate( INT Size );


/*** Render support ***/

extern MATR
  AL5_RndMatrWorld,
  AL5_RndMatrView,
  AL5_RndMatrProj;

extern DBL
  AL5_RndProjDist,
  AL5_RndFarClip,
  AL5_RndProjSize;

/* Vertex representation type */
typedef struct
{
  VEC  P;  /* Vertex position */
  VEC2 T;  /* Vertex texture coordinates */
  VEC  N;  /* Normal at vertex */
  VEC4 C;  /* Vertex color */
} al5VERTEX;

typedef struct
{
  al5VERTEX *V;     /* Primitive vertex array */
  INT NumOfV;       /* Point array size */
  INT *I;           /* Facets index array */
  INT NumOfI;       /* Facets index array size */
} al5PRIM;

VOID AL5_RndPrimDraw( al5PRIM *Pr );
MATR MatrView( VEC Loc, VEC Add, VEC Up );