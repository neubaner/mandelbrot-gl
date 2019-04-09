#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>

#include "gl_linux.h"
#include "gl_linux.cpp"

int main(int argc, char** args){
  Display                 *dpy;
  Window                  root;
  GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
  XVisualInfo             *vi;
  Colormap                cmap;
  XSetWindowAttributes    swa;
  Window                  win;
  GLXContext              glc;
  XWindowAttributes       gwa;
  XEvent                  xev;

  dpy = XOpenDisplay(NULL);

  if(dpy == NULL){
    printf("Could not get X11 Display\n");
    exit(0);
  }

  root = DefaultRootWindow(dpy);
  vi = glXChooseVisual(dpy, 0, att);
  cmap = XCreateColormap(dpy, vi->visual, AllocNone);
  swa.colormap = cmap;
  swa.event_mask = KeyPressMask | KeyReleaseMask;

  win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
  XMapWindow(dpy, win);
  XStoreName(dpy, win, "MandelGL");

  glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
  glXMakeCurrent(dpy, win, glc);

  loadGlProcs();

  while(1) {
    XNextEvent(dpy, &xev);

    if(xev.type == KeyPress){

    } else if(xev.type == KeyRelease) {

    }

    // We draw;
  }

  return 0;
}
