/* FILE NAME: ANIM.C
 * PROGRAMMER: AL5
 * DATE: 10.06.2016
 * PURPOSE: WinAPI windowed application sample.
 */

#include <stdio.h>

#include "ANIM.H"

#pragma comment(lib, "winmm")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

#define AL5_GET_JOYSTIC_AXIS(A)  (2.0 * (ji.dw##A##pos - jc.w##A##min) / (jc.w##A##max - jc.w##A##min - 1) - 1)

INT AL5_MOUSEWHEEL;
al5ANIM AL5_Anim;
static UINT64
  AL5_StartTime,    /* Start program time */
  AL5_OldTime,      /* Time from program start to previous frame */
  AL5_OldTimeFPS,   /* Old time FPS measurement */
  AL5_PauseTime,    /* Time during pause period */
  AL5_TimePerSec,   /* Timer resolution */
  AL5_FrameCounter; /* Frames counter */

VOID AL5_AnimInit( HWND hWnd )
{ 
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  LARGE_INTEGER t;
  
 
  AL5_Anim.NumOfUnits = 0;
  
  memset(&AL5_Anim, 0, sizeof(al5ANIM));
  AL5_Anim.hWnd = hWnd;
  AL5_Anim.hDC = GetDC(hWnd);

    /*** openGl ***/
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

    /*** init timer ***/
  QueryPerformanceFrequency(&t);
  AL5_TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  AL5_StartTime = AL5_OldTime = AL5_OldTimeFPS = t.QuadPart;
  AL5_PauseTime = 0;

  AL5_RndMatrProj = MatrFrustum(-1, 1, -1, 1, 1, 100);
  AL5_RndMatrWorld = MatrIdentity();
  AL5_RndMatrView  = MatrMulMatr(MatrIdentity(), MatrTranslate(VecSet(-1, -1, 0)));

  /* OpenGL specific initialization */
  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST );
  /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */

  AL5_RndPrg = AL5_RndShaderLoad("a");
}

VOID AL5_AnimResize( INT w,INT h )
{
  AL5_Anim.W = w;
  AL5_Anim.H = h;
  glViewport(0, 0, w, h);
  AL5_RndProj();
}

VOID AL5_AnimCopyFrame( VOID )
{
  SwapBuffers(AL5_Anim.hDC);
}

VOID AL5_AnimAddUnit( al5UNIT *Uni )
{
  if(AL5_Anim.NumOfUnits < AL5_MAX_UNITS)
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
  /* Delete rendering context */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AL5_Anim.hGLRC);

  /* Delete GDI data */
  ReleaseDC(AL5_Anim.hWnd, AL5_Anim.hDC);
  DeleteDC(AL5_Anim.hDC);
  KillTimer(AL5_Anim.hWnd, 0);
  memset(&AL5_Anim, 0, sizeof(al5ANIM));
  PostQuitMessage(0);
}

VOID AL5_AnimRender( VOID )
{
  INT i;
  static FLT dx = 0, dy = 0;
  LARGE_INTEGER t;
  POINT pt;

  /* Mouse wheel */
  AL5_Anim.Mdz = AL5_MOUSEWHEEL;
  AL5_Anim.Mz += AL5_MOUSEWHEEL;
  AL5_MOUSEWHEEL = 0;

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(AL5_Anim.hWnd, &pt);
  AL5_Anim.Mdx = pt.x - AL5_Anim.Mx;
  AL5_Anim.Mdy = pt.y - AL5_Anim.My;
  AL5_Anim.Mx = pt.x;
  AL5_Anim.My = pt.y;

  /*Keyboard*/
  GetKeyboardState(AL5_Anim.Keys);
  for ( i = 0; i < 256; i++ )
  {
    AL5_Anim.Keys[i] >>= 7;
    if (!AL5_Anim.OldKeys[i] && AL5_Anim.Keys[i])
      AL5_Anim.KeysClick[i] = 1;
    else
      AL5_Anim.KeysClick[i] = 0;
  }
  memcpy(AL5_Anim.OldKeys, AL5_Anim.Keys, 256);

  /* joystick */
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons */
        for (i = 0; i < 32; i++)
          AL5_Anim.JBut[i] = (ji.dwButtons >> i) & 1;

        /* Axes */
        AL5_Anim.JX = AL5_GET_JOYSTIC_AXIS(X);
        AL5_Anim.JY = AL5_GET_JOYSTIC_AXIS(Y);
        AL5_Anim.JZ = AL5_GET_JOYSTIC_AXIS(Z);
        AL5_Anim.JR = AL5_GET_JOYSTIC_AXIS(R);

        /* Point of view */
        AL5_Anim.JPov = ji.dwPOV == 0xFFFF ? 0 : ji.dwPOV / 4500 + 1;
      }
    }
  }

  /*** Handle timer ***/
  AL5_FrameCounter++;
  QueryPerformanceCounter(&t);
  
  /* Global time */
  AL5_Anim.GlobalTime = (FLT)(t.QuadPart - AL5_StartTime) / AL5_TimePerSec;
  AL5_Anim.GlobalDeltaTime = (FLT)(t.QuadPart - AL5_OldTime) / AL5_TimePerSec;
  
  /* Time with pause */
  if (AL5_Anim.IsPause)
  {
    AL5_Anim.DeltaTime = 0;
    AL5_PauseTime += t.QuadPart - AL5_OldTime;
  }
  else
  {
    AL5_Anim.DeltaTime = AL5_Anim.GlobalDeltaTime;
    AL5_Anim.Time = (FLT)(t.QuadPart - AL5_PauseTime - AL5_StartTime) / AL5_TimePerSec;
  }

  /* FPS */
  if (t.QuadPart - AL5_OldTimeFPS > AL5_TimePerSec)
  {
    CHAR str[100];

    AL5_Anim.FPS = AL5_FrameCounter * AL5_TimePerSec /
                                         (FLT)(t.QuadPart - AL5_OldTimeFPS);
    AL5_OldTimeFPS = t.QuadPart;
    AL5_FrameCounter = 0;
    sprintf(str, "Anim FPS: %.5f Mouse Coord:  %i, %i JoyStick Coord: %f %f; Number of units: %i", AL5_Anim.FPS, AL5_Anim.Mx, AL5_Anim.My, AL5_Anim.JX,AL5_Anim.JX, AL5_Anim.NumOfUnits - 1);
    SetWindowText(AL5_Anim.hWnd, str);
  }
  AL5_OldTime = t.QuadPart;  

  for (i = 0; i < AL5_Anim.NumOfUnits; i++)  
    AL5_Anim.Units[i]->Response(AL5_Anim.Units[i], &AL5_Anim);
   /*** Clear frame ***/
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for (i = 0; i < AL5_Anim.NumOfUnits; i++)  
  {
    AL5_RndMatrWorld = MatrIdentity();
    AL5_Anim.Units[i]->Render(AL5_Anim.Units[i], &AL5_Anim);
  }

  glLoadMatrixf(&AL5_RndMatrView.A[0][0]);
  glFinish();
}