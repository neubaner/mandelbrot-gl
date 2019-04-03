#include <windows.h>
#include <stdio.h>
#include "gl.cpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

LRESULT CALLBACK WindowEventHandler(HWND window, UINT msg, WPARAM  wParam, LPARAM  lParam){
  switch(msg){
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProcA(window, msg, wParam, lParam);
  }

  return 0;
}

HWND createWindow(const char* className, HINSTANCE instance, int width, int height){
  return CreateWindowA(
      className,
      "MandelGL",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
	    width,
      height,
      NULL,
      NULL,
      instance,
      NULL
    );
}

void setupOpenGL(HINSTANCE instance){
  WNDCLASSA windowClass;
  windowClass.style = CS_OWNDC;
  windowClass.lpfnWndProc = NULL;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = instance;
  windowClass.hIcon = NULL;
  windowClass.hCursor = NULL;
  windowClass.hbrBackground = NULL;
  windowClass.lpszMenuName = "DumbGLWindow";
  windowClass.lpszClassName = windowClass.lpszMenuName;

  HWND dumbWindow = createWindow(windowClass.lpszClassName, instance, 0, 0);
  HDC dc = GetDC(dumbWindow);

  // Create a dumb GL context to get all gl/extensions procedures
  // From: https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
  PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
    PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
    32,                   // Colordepth of the framebuffer.
    0, 0, 0, 0, 0, 0,
    0,
    0,
    0,
    0, 0, 0, 0,
    24,                   // Number of bits for the depthbuffer
    8,                    // Number of bits for the stencilbuffer
    0,                    // Number of Aux buffers in the framebuffer.
    PFD_MAIN_PLANE,
    0,
    0, 0, 0
  };

  int dummyPixelFormat = ChoosePixelFormat(dc, &pfd);
  SetPixelFormat(dc, dummyPixelFormat, &pfd);

  HGLRC glCtx = wglCreateContext(dc);
  wglMakeCurrent(dc, glCtx);

  loadGlProcs();

  wglDeleteContext(glCtx);
  ReleaseDC(dumbWindow, dc);
  DestroyWindow(dumbWindow);
}

int findPixelFormat(HDC dc){
  const int attribList[] = {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 24,
    WGL_STENCIL_BITS_ARB, 8,
    0, // End
  };

  int pixelFormat;
  UINT matchingFormats;
  if (wglChoosePixelFormatARB(dc, attribList, NULL, 1, &pixelFormat, &matchingFormats) == FALSE) {
	  printf("Could not get pixelFormat %d", GetLastError());
	  abort();
  }

  return pixelFormat;
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd){
  WNDCLASSA windowClass;
  windowClass.style = CS_OWNDC;
  windowClass.lpfnWndProc = WindowEventHandler;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = instance;
  windowClass.hIcon = NULL;
  windowClass.hCursor = NULL;
  windowClass.hbrBackground = NULL;
  windowClass.lpszMenuName = "MandelGlWindow";
  windowClass.lpszClassName = windowClass.lpszMenuName;
  
  if(RegisterClassA(&windowClass) == 0){
    printf("Error registering window class %d\n", GetLastError());
    return -1;
  }

  setupOpenGL(instance);

  HWND window = createWindow(windowClass.lpszClassName, instance, 800, 600);
   
  if(window == NULL){
    printf("Error creating window %d", GetLastError());
    return -1;
  }

  HDC dc = GetDC(window);
  int pixelFormat = findPixelFormat(dc);

  PIXELFORMATDESCRIPTOR pfd;
  DescribePixelFormat(dc, pixelFormat, sizeof(pfd), &pfd);
  SetPixelFormat(dc, pixelFormat, &pfd);
  
  HGLRC glCtx = wglCreateContext(dc);
  wglMakeCurrent(dc, glCtx);

  ShowWindow(window, nShowCmd);
  UpdateWindow(window);

  MSG msg;
  bool running = true;
  while(running)
  {
    while(PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
      if (msg.message == WM_QUIT) {
        running = false;
		break;
      } else {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
      }
    }

    glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SwapBuffers(dc);
  }

  return msg.wParam;
}


int main(){
  return WinMain(
    (HINSTANCE)GetModuleHandle(NULL),
    NULL,
    GetCommandLineA(),
    SW_SHOW
  );
}
