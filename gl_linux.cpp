#include "gl_linux.h"

void* loadGlProc(const char* name){
    return glXGetProcAddress((const GLubyte *)name);
}
