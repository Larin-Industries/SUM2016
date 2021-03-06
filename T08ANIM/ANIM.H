/* FILE NAME: ANIM.H
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "def.H"

#define AL5_MAX_UNITS 100

UINT AL5_RndPrg;

typedef struct tagal5UNIT al5UNIT;
typedef struct
{
  CREATESTRUCT *cs;
  HWND hWnd; 
  HDC hDC;   
  INT W, H;  
  HGLRC hGLRC;                   /* Rendering context */
  al5UNIT *Units[AL5_MAX_UNITS]; 
  INT NumOfUnits;
  /*** Timer ***/
  FLT GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
      Time, DeltaTime,             /* Time with pause and interframe interval */
      FPS;                         /* Frames per seond value */
  BOOL IsPause;                     /* Pause flag */

    /***  Mouse   ***/
  INT Mdx, Mdy, Mdz,
      Mx,  My,  Mz;

  /*** KeyBoard ***/
  BYTE Keys[256], OldKeys[256], KeysClick[256];

  /*** JoyStick ***/
  BYTE JBut[32];                    /* Joystick button state */
  INT JPov;                      /* Joystick point-of-view control [0,1..8] */
  FLT JX, JY, JZ, JR;              /* Joystick axes */
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
  VOID (* Init)( al5UNIT *Uni, al5ANIM *Ani );

  /* Unit deinitialization function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       al5UNIT *Uni;
   *   - animation context:
   *       al5ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Close)( al5UNIT *Uni, al5ANIM *Ani );

  /* Unit inter frame events handle function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       al5UNIT *Uni;
   *   - animation context:
   *       al5ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Response)( al5UNIT *Uni, al5ANIM *Ani );

  /* Unit render function.
   * ARGUMENTS:
   *   - self-pointer to unit object:
   *       al5UNIT *Uni;
   *   - animation context:
   *       al5ANIM *Ani;
   * RETURNS: None.
   */
  VOID (* Render)( al5UNIT *Uni, al5ANIM *Ani );
};


typedef struct
{
//  al5VERTEX *V;     /* Primitive vertex array */
//  INT NumOfV;       /* Point array size */
//  INT *I;           /* Facets index array */
  INT VA;
  INT IBuf;
  INT VBuf;
  INT NumOfI;       /* Facets index array size */
} al5PRIM;

extern MATR 
  AL5_RndMatrWorld/* = MatrIdentity()*/,
  AL5_RndMatrView /* = MatrMulMatr(MatrIdentity, MatrTranslate(VecSet(-1, -1, 0)))*/,
  AL5_RndMatrProj;

extern FLT  
  AL5_RndProjDist,
  AL5_RndFarClip,
  AL5_RndProjSize;

extern INT AL5_MOUSEWHEEL;
extern al5ANIM AL5_Anim;

VOID AL5_AnimInit( HWND hWnd );
VOID AL5_AnimResize( INT w,INT h );
VOID AL5_AnimCopyFrame( VOID );
VOID AL5_AnimAddUnit( al5UNIT *Uni );
VOID AL5_AnimClose( VOID );
VOID AL5_AnimRender( VOID );
VOID AL5_RndProj( VOID );
VOID FlipFullScreen( HWND hWnd );
BOOL AL5_RndPrimLoad( al5PRIM *Pr, CHAR *FileName );
VOID AL5_RndPrimFree( al5PRIM *Pr );

al5UNIT * AL5_AnimUnitCreate( INT Size );

/* Shaders functions */
UINT AL5_RndShaderLoad( CHAR *FileNamePrefix );
VOID AL5_RndShaderFree( UINT Prg );