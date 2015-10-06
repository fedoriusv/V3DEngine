@echo off
if not exist project\win32 mkdir project\win32
cd project\win32
cmake -DCOMPILER_MSVC=ON -G"Visual Studio 14 Win64" ../..
pause