/* FILE NAME: ANIM.C
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include "ANIM.H"

#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

al5ANIM AL5_Anim;

INT AL5_MouseWheel;

VOID AL5_AnimInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&AL5_Anim, 0, sizeof(al5ANIM));

  /* Store window and create memory device context */
  AL5_Anim.hWnd = hWnd;
  AL5_Anim.hDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AL5_Anim.hDC, &pfd);
  DescribePixelFormat(AL5_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AL5_Anim.hDC, i, &pfd);
  
  /* OpenGL init: setup rendering context */
  AL5_Anim.hGLRC = wglCreateContext(AL5_Anim.hDC);
  wglMakeCurrent(AL5_Anim.hDC, AL5_Anim.hGLRC);

  /* OpenGL init: setup extensions: GLEW library */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(AL5_Anim.hGLRC);
    ReleaseDC(AL5_Anim.hWnd, AL5_Anim.hDC);
    exit(0);
  }

  AL5_RndMatrView = MatrView(VecSet(5, 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));

  /* OpenGL specific initialization */
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
}

VOID AL5_AnimResize( INT W, INT H )
{
  AL5_Anim.H = H;
  AL5_Anim.W = W;

  glViewport(0, 0, W, H);

  AL5_RndSetProj();
}

VOID AL5_AnimCopyFrame( VOID )
{
  SwapBuffers(AL5_Anim.hDC);
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

  DeleteObject(AL5_Anim.hFrame);

    /* Delete rendering context */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AL5_Anim.hGLRC);

  /* Delete GDI data */
  ReleaseDC(AL5_Anim.hWnd, AL5_Anim.hDC);
}

VOID AL5_AnimFlipFullscreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (IsFullScreen)
  {
    SetWindowPos(AL5_Anim.hWnd, HWND_TOP, SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    RECT rc;

    GetWindowRect(AL5_Anim.hWnd, &SaveRect);
    hmon = MonitorFromWindow(AL5_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(AL5_Anim.hWnd, GWL_STYLE), FALSE);
    SetWindowPos(AL5_Anim.hWnd, HWND_TOPMOST, 
      rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
  }
  IsFullScreen = !IsFullScreen;
}

VOID AL5_AnimRender( VOID )
{
  static camX = 5, camY;
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

  for (i = 0; i < AL5_Anim.NumOfUnits; i++)
  {
    SelectObject(AL5_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AL5_Anim.hDC, GetStockObject(NULL_PEN));
    SetDCBrushColor(AL5_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AL5_Anim.hDC, RGB(0, 0, 0));
    AL5_Anim.Units[i]->Render(AL5_Anim.Units[i], &AL5_Anim);
  }

  /*** Clear frame ***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /*** Render all units ***/
  for (i = 0; i < AL5_Anim.NumOfUnits; i++)
  {
    AL5_RndMatrWorld = MatrIdentity();
    AL5_Anim.Units[i]->Render(AL5_Anim.Units[i], &AL5_Anim);
  }

  /* Finalize OpenGL drawing */
  glFinish();
}