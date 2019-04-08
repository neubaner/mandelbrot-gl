<<<<<<< HEAD
@echo off
cl /Z7 main.cpp /link user32.lib gdi32.lib opengl32.lib /SUBSYSTEM:CONSOLE
del main.obj
del main.ilk
del main.pdb
=======
cl /Z7 /Fe:build\ /Fo:build\ platform_windows.cpp /link user32.lib gdi32.lib opengl32.lib /SUBSYSTEM:CONSOLE
>>>>>>> 55d16d9... prep for platform independent code
