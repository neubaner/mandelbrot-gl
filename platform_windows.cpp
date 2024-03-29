#include <windows.h>
#include <stdio.h>

#include "gl_windows.h"

#include "gl_windows.cpp"
#include "mandel.cpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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

LRESULT CALLBACK DumbWindowEventHandler(HWND window, UINT msg, WPARAM  wParam, LPARAM  lParam) {
	return DefWindowProcA(window, msg, wParam, lParam);
}

void setupOpenGL(HINSTANCE instance){
  WNDCLASSA windowClass;
  windowClass.style = CS_OWNDC;
  windowClass.lpfnWndProc = DumbWindowEventHandler;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = instance;
  windowClass.hIcon = NULL;
  windowClass.hCursor = NULL;
  windowClass.hbrBackground = NULL;
  windowClass.lpszClassName = "DumbGLWindow";
  windowClass.lpszMenuName = "DumbGLWindow";
  RegisterClassA(&windowClass);

  HWND dumbWindow = createWindow("DumbGLWindow", instance, 0, 0);
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
  DescribePixelFormat(dc, dummyPixelFormat, sizeof(pfd), &pfd);
  if(SetPixelFormat(dc, dummyPixelFormat, &pfd) == FALSE) {
    printf("Error Settings pixel format %x", GetLastError());
    abort();
  }

  HGLRC glCtx = wglCreateContext(dc);
  if (glCtx == NULL) {
    printf("Could not create OpenGL Context %d", GetLastError());
  }

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

void setMandelKeys(char* keys, WPARAM wParam, bool value){
  switch(wParam){
    case VK_LEFT:
    case 'A': {
      keys[MandelKeyLeft] = value;
    } break;

    case VK_RIGHT:
    case 'D': {
      keys[MandelKeyRight] = value;
    } break;

    case VK_UP:
    case 'W': {
      keys[MandelKeyUp] = value;
    } break;

    case VK_DOWN:
    case 'S': {
      keys[MandelKeyDown] = value;
    } break;

    case VK_ADD: {
      keys[MandelKeyZoomIn] = value;
    } break;

    case VK_SUBTRACT: {
      keys[MandelKeyZoomOut] = value;
    } break;
  }
}

LRESULT CALLBACK WindowEventHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam){
  switch(msg){
    case WM_KEYDOWN: {
      char* keys = (char *)GetWindowLongPtr(window, GWLP_USERDATA);
      setMandelKeys(keys, wParam, true);   
      break;
    }

    case WM_KEYUP: {
      char* keys = (char *)GetWindowLongPtr(window, GWLP_USERDATA);
      setMandelKeys(keys, wParam, false);
      break;
    }

    case WM_DESTROY: {
      PostQuitMessage(0);
      break;
    }

    default:
      return DefWindowProcA(window, msg, wParam, lParam);
  }

  return 0;
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd){
  char keys[MandelKeyCount] = {};

  setupOpenGL(instance);

  WNDCLASSA windowClass;
  windowClass.style = CS_OWNDC;
  windowClass.lpfnWndProc = WindowEventHandler;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = NULL;
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

  const int screenWidth = 600;
  const int screenHeight = 600;

  HWND window = createWindow(windowClass.lpszClassName, instance, screenWidth, screenHeight);
   
  if(window == NULL){
    printf("Error creating window %d", GetLastError());
    return -1;
  }

  SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)keys);
  HDC dc = GetDC(window);
  int pixelFormat = findPixelFormat(dc);

  PIXELFORMATDESCRIPTOR pfd;
  DescribePixelFormat(dc, pixelFormat, sizeof(pfd), &pfd);
  SetPixelFormat(dc, pixelFormat, &pfd);
  
  HGLRC glCtx = wglCreateContext(dc);
  wglMakeCurrent(dc, glCtx);

  ShowWindow(window, nShowCmd);
  UpdateWindow(window);
  RenderMandelData data = setupRenderMandel();

  double zoom = 1;
  double offsetX = 0;
  double offsetY = 0;

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

    for(int i=0; i<MandelKeyCount; i++){
      if(keys[i]){
        mandelHandleInput(&data, (MandelKey)i);
      }
    }

    glClearColor(0.5, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BITS);
    renderMandel(&data, screenWidth, screenHeight);
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