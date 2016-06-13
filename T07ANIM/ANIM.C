/* FILE NAME: ANIM.C
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "ANIM.H"

al5ANIM AL5_Anim;

INT AL5_MouseWheel;

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

  AL5_RndMatrWorld = MatrIdentity();
  AL5_RndMatrView = MatrView(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  AL5_RndMatrProj = Frustum(-1, 1, -1, 1, 1, 100);
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

  AL5_RndSetProj();
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
  POINT pt;

  /*** Obtain input system state ***/

  /* Mouse */

  GetCursorPos(&pt);
  ScreenToClient(AL5_Anim.hWnd, &pt);
  AL5_Anim.Mx = pt.x;
  AL5_Anim.My = pt.y;

  AL5_Anim.Mdx = pt.x - AL5_Anim.MOldx;
  AL5_Anim.Mdy = pt.y - AL5_Anim.MOldy;
  AL5_Anim.MOldx = AL5_Anim.Mx;
  AL5_Anim.MOldy = AL5_Anim.My;

  GetKeyboardState(AL5_Anim.Keys);

  AL5_Anim.Mdz = AL5_MouseWheel;
  AL5_Anim.Mz = AL5_MouseWheel;
  AL5_MouseWheel = 0;

  /* Keyboard */
  
  GetKeyboardState(AL5_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AL5_Anim.Keys[i] >>= 7;
    if (!AL5_Anim.KeysOld[i] && AL5_Anim.Keys[i])
      AL5_Anim.KeysOld[i] = TRUE;
    else
      AL5_Anim.KeysOld[i] = FALSE;
  }

  memcpy(AL5_Anim.KeysOld, AL5_Anim.Keys, 256);

  /* Joystick */

  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;
      
      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        for (i = 0; i < 32; i++)
          AL5_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        AL5_Anim.JX = 2.0 * (ji.dwXpos - jc.wXmin) / (jc.wXmax - jc.wXmin - 1) - 1;
        AL5_Anim.JY = 2.0 * (ji.dwYpos - jc.wYmin) / (jc.wYmax - jc.wYmin - 1) - 1;
        AL5_Anim.JZ = 2.0 * (ji.dwZpos - jc.wZmin) / (jc.wZmax - jc.wZmin - 1) - 1;
        AL5_Anim.JR = 2.0 * (ji.dwRpos - jc.wRmin) / (jc.wRmax - jc.wRmin - 1) - 1;

        AL5_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

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