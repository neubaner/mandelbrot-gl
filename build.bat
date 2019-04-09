@echo off
cl /Z7 platform_windows.cpp /link user32.lib gdi32.lib opengl32.lib /SUBSYSTEM:WINDOWS