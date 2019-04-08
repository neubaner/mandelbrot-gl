#ifndef PLATFORM_WINDOWS_GL_H
#define PLATFORM_WINDOWS_GL_H

#define PREFIX_GL_PROC WINAPI
#define LIST_OS_GL_PROCS(Z) \
    Z(BOOL WINAPI,      wglChoosePixelFormatARB,    HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats)

#include "gl.h"

#endif 