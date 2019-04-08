void* loadGlProc(const char* name){
    void* proc = (void *)wglGetProcAddress(name);
    if(proc == (void *)0 || proc == (void *)1 ||
       proc == (void *)2 || proc == (void *)3 || proc == (void *)-1){
        HMODULE module = LoadLibraryA("opengl32.dll");
        proc = GetProcAddress(module, name);
    }

    return proc;
}
