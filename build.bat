<<<<<<< HEAD
@echo off
cl /Z7 platform_windows.cpp /link user32.lib gdi32.lib opengl32.lib /SUBSYSTEM:CONSOLE
del main.obj
del main.ilk
del main.pdb
=======
cl /Z7 /Fe:build\ /Fo:build\ platform_windows.cpp /link user32.lib gdi32.lib opengl32.lib /SUBSYSTEM:CONSOLE
>>>>>>> 8fdaea9f73e63a9c3350fb2db48b01e40e749d9d
