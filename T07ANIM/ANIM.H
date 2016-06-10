/* FILE NAME: ANIM.H
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "DEF.H"

#define AL5_MAX_UNITS 1000000

typedef struct tagal5UNIT al5UNIT;

typedef struct
{
  HWND hWnd; 
  HDC hDC;   
  INT W, H;  
  HBITMAP hFrame;
  al5UNIT *Units[AL5_MAX_UNITS];
  INT NumOfUnits;
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