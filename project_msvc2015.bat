@echo off
if not exist project\win32 mkdir project\win
cd project\win
cmake -DCOMPILER_MSVC=ON -G"Visual Studio 14 Win64" ../..
pause