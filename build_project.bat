@echo ON 
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
echo "Starting Build for all Projects with proposed changes"
echo "V3DEngine builds started..." 
devenv "%CD%\project\win\Valera3D.sln" /build "Debug|x64"
devenv "%CD%\project\win\Valera3D.sln" /build "Release|x64"
devenv "%CD%\project\win\Valera3D.sln" /build "F3DDebug|x64"
devenv "%CD%\project\win\Valera3D.sln" /build "F3DRelease|x64"
echo "V3DEngine builds finished..." 
echo "All builds completed."
pause