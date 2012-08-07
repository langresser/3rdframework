/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2007, 2009, 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#include <GLES/gl.h>
#include <EGL/egl.h>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <stdio.h>


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
        "OGLES", "Rotate triangle example running on OpenGL ES 1.1 Emulator", 
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
        0, 0, 
        wRect.right-wRect.left, wRect.bottom-wRect.top, 
        NULL, NULL, 
        hInstance, 
        NULL
        );
    *result = hwnd;
}

int main(void) {
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
    unsigned char mIndices[] = { 0, 1, 2 };
    signed short mVertices[] = {
        -50, -29, 0,
        50, -29, 0,
        0,  58, 0
    };
    HWND hwnd;
    HDC hdc;
    MSG sMessage;
    int bDone = 0;

    // Platform init.
    platform(&hwnd, 640, 480);
    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    // EGL init.
    hdc = GetDC(hwnd);
    m_eglDisplay = eglGetDisplay(hdc);
    eglInitialize(m_eglDisplay, NULL, NULL);
    eglChooseConfig(m_eglDisplay, aEGLAttributes, m_eglConfig, 1, &nConfigs);
    printf("EGLConfig = %p\n", m_eglConfig[0]);
    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig[0], (NativeWindowType)hwnd, 0);
    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig[0], EGL_NO_CONTEXT, aEGLContextAttributes);
    printf("EGLContext = %p\n", m_eglContext);
    eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_SHORT, 0, mVertices);

    /* Set projection matrix so screen extends to (-160, -120) at bottom left 
    * and to (160, 120) at top right, with -128..128 as Z buffer. */

    glMatrixMode(GL_PROJECTION);
    glOrthox(-160<<16, 160<<16, -120<<16, 120<<16, -128<<16, 128<<16);

    glMatrixMode(GL_MODELVIEW);

    glClearColorx(0x10000, 0x10000, 0, 0);
    glColor4x(0x10000, 0, 0, 0);

    // Main event loop
    while(!bDone)
    {
        // Do Windows stuff:
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

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);
        glRotatex(2<<16, 0, 0, 0x10000);
        eglSwapBuffers(m_eglDisplay, m_eglSurface);
        Sleep(30);
    }

    // Exit.
    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(m_eglDisplay, m_eglContext);
    eglDestroySurface(m_eglDisplay, m_eglSurface);
    eglTerminate(m_eglDisplay);

    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);


    return 0;
}
