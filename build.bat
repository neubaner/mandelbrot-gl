@echo off
cl /Z7 main.cpp /link user32.lib gdi32.lib opengl32.lib /SUBSYSTEM:CONSOLE
del main.obj
del main.ilk
del main.pdb