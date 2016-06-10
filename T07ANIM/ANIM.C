/* FILE NAME: ANIM.C
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "ANIM.H"

al5ANIM AL5_Anim;

VOID AL5_AnimInit( HWND hWnd )
{
  HDC hDC;

  memset(&AL5_Anim, 0, sizeof(al5ANIM));

  /* store window and create memory device context*/ 
  AL5_Anim.hWnd = hWnd;
  hDC = GetDC(hWnd);
  AL5_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  AL5_Anim.NumOfUnits = 0;
}

VOID AL5_AnimResize( INT W, INT H )
{
  HDC hDC;

  AL5_Anim.H = H;
  AL5_Anim.W = W;

  if (AL5_Anim.hFrame != NULL)
    DeleteObject(AL5_Anim.hFrame);
  
  hDC = GetDC(AL5_Anim.hWnd);

  AL5_Anim.hFrame = CreateCompatibleBitmap(hDC, W, H);

  ReleaseDC(AL5_Anim.hWnd, hDC);
  SelectObject(AL5_Anim.hDC, AL5_Anim.hFrame);
}

VOID AL5_AnimCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, AL5_Anim.W, AL5_Anim.H, AL5_Anim.hDC, 0, 0, SRCCOPY);
}

VOID AL5_AnimAddUnit( al5UNIT *Uni )
{
  if (AL5_Anim.NumOfUnits < AL5_MAX_UNITS)
  {
    AL5_Anim.Units[AL5_Anim.NumOfUnits++] = Uni;
    Uni->Init(Uni, &AL5_Anim); 
  }
}

VOID AL5_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < AL5_Anim.NumOfUnits; i++)
  {
    AL5_Anim.Units[i]->Close(AL5_Anim.Units[i], &AL5_Anim);
    free(AL5_Anim.Units[i]);
  }

  AL5_Anim.NumOfUnits = 0;

  DeleteDC(AL5_Anim.hDC);
  DeleteObject(AL5_Anim.hFrame);
}

VOID AL5_AnimRender( VOID )
{
  INT i;

  /*** Obtain input system state ***/

  /* Keyboard */

  /* Mouse */
  
  /* Joystick */

  for (i = 0; i < AL5_Anim.NumOfUnits; i++)
    AL5_Anim.Units[i]->Response(AL5_Anim.Units[i], &AL5_Anim);

  InvalidateRect(AL5_Anim.hWnd, NULL, FALSE); 

  SelectObject(AL5_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AL5_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AL5_Anim.hDC, RGB(100, 155, 220));
  Rectangle(AL5_Anim.hDC, 0, 0, AL5_Anim.W + 1, AL5_Anim.H + 1);

  for (i = 0; i < AL5_Anim.NumOfUnits; i++)
  {
    SelectObject(AL5_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AL5_Anim.hDC, GetStockObject(NULL_PEN));
    SetDCBrushColor(AL5_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AL5_Anim.hDC, RGB(0, 0, 0));
    AL5_Anim.Units[i]->Render(AL5_Anim.Units[i], &AL5_Anim);
  }
} 