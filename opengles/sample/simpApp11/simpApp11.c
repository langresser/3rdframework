/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2007, 2009, 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/* file simpApp11.c  Color Cube example using OpenGL ES 1.1 */

#include <stdio.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <math.h>
#include <time.h>

static const GLfloat coord [] = {
				  1.f,  1.f,  1.f,      /* 0 */
				 -1.f,  1.f,  1.f,      /* 1 */
				 -1.f, -1.f,  1.f,      /* 2 */
				  1.f, -1.f,  1.f,      /* 3 */
				 -1.f,  1.f, -1.f,      /* 4 */
				  1.f,  1.f, -1.f,      /* 5 */
				  1.f, -1.f, -1.f,      /* 6 */
				 -1.f, -1.f, -1.f };    /* 7 */

static const GLubyte color [] = {
				 255, 255, 255, 255, 
				 255,   0, 255, 255, 
				 0,     0, 255, 255, 
				 0,   255, 255, 255,
				 255,   0,   0, 255,
				 255, 255,   0, 255,
				 0,   255,   0, 255,
				 0,     0,   0, 255};

static const GLushort fanIx [] = {
				 0, 1, 2, 3, 6, 5, 4, 1,   /* One tri.fan  */
				 7, 6, 3, 2, 1, 4, 5, 6};  /*  and another */


static void init(void) {
    glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
    glClearDepthf(1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 256, 256);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-0.6f, 0.6f, -0.6f, 0.6f, 2.0f, 10.f);
    glMatrixMode(GL_MODELVIEW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, coord);
    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, color);
}  /* init */


static float tim(void) {
    return (float)clock()/(float)CLOCKS_PER_SEC;
}


static void draw(void) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
		printf("draw() failed (error 0x%x)\n", err);
		exit(1);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -6.0f);
    glRotatef(tim()*55, 0.3f, 1.f, 1.3f);
    glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_SHORT, fanIx);
    glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_SHORT, &(fanIx[8]));
}  /* draw */

LRESULT CALLBACK process_window(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
    switch(uiMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        case WM_ACTIVATE:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SIZE:
            return 0;
    }

    return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

void platform(HWND *result, int width, int height) {
  WNDCLASS wc;
  RECT wRect;
  HWND hwnd;
  HINSTANCE hInstance;

  wRect.left = 0L;
  wRect.right = (long)width;
  wRect.top = 0L;
  wRect.bottom = (long)height;

  hInstance = GetModuleHandle(NULL);

  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = (WNDPROC)process_window;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "OGLES";

  RegisterClass(&wc);

  AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

  hwnd = CreateWindowEx(
      WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, 
      "OGLES", "Spinning Cube example",
      WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
      0, 0, 
      wRect.right-wRect.left, wRect.bottom-wRect.top, 
      NULL, NULL, 
      hInstance, 
      NULL
      );
  *result = hwnd;
}

int main(int argc, char** argv) {

    EGLDisplay m_eglDisplay;
    EGLContext m_eglContext;
    EGLSurface m_eglSurface;
    EGLint attributeList[] = { EGL_RED_SIZE, 1, EGL_DEPTH_SIZE, 1, EGL_NONE };
    
    EGLint		aEGLAttributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_NONE
    };
    EGLint		aEGLContextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 1,
        EGL_NONE
    };
    EGLConfig m_eglConfig[1];
    EGLint nConfigs;

    HWND hwnd;
    HDC hdc;
    MSG sMessage;
    int bDone = 0;

    /* Platform init. */
    platform(&hwnd, 256, 256);
    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    /* EGL init. */
    hdc = GetDC(hwnd);
    m_eglDisplay = eglGetDisplay(hdc);

    eglInitialize(m_eglDisplay, NULL, NULL);
    eglChooseConfig(m_eglDisplay, aEGLAttributes, m_eglConfig, 1, &nConfigs);

    printf("EGLConfig = %p\n", m_eglConfig[0]);

    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig[0], (NativeWindowType)hwnd, 0);
    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig[0], EGL_NO_CONTEXT, aEGLContextAttributes);

    printf("EGLContext = %p\n", m_eglContext);

    eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

    /* Begin user code: */

    init();
    for (;!bDone;)
    {
        /* Do Windows stuff */
        if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE))
        {
            if(sMessage.message == WM_QUIT)
            {
                bDone = 1;
            }
            else 
            {
                TranslateMessage(&sMessage);
                DispatchMessage(&sMessage);
            }
        }
        draw();
        eglSwapBuffers(m_eglDisplay, m_eglSurface);
    }		
    
    /* End of user code */

    /* Exit and clean up */
    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(m_eglDisplay, m_eglContext);
    eglDestroySurface(m_eglDisplay, m_eglSurface);
    eglTerminate(m_eglDisplay);

    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
    return 0;
}