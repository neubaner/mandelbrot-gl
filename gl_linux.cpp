#include "gl_linux.h"

void* loadGlProc(const char* name){
    return (void *)glXGetProcAddress((const GLubyte *)name);
}
