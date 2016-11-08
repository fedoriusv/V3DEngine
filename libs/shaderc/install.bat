@echo off
git clone https://github.com/google/shaderc

cd shaderc/third_party
REM  git clone https://github.com/google/googletest.git
git clone https://github.com/google/glslang.git
git clone https://github.com/KhronosGroup/SPIRV-Tools.git spirv-tools
git clone https://github.com/KhronosGroup/SPIRV-Headers.git spirv-tools/external/spirv-headers
cd ../

cmake -G"Visual Studio 14 2015 Win64" -DSHADERC_SKIP_TESTS=ON 
REM cmake --build . --config {Debug}
REM cmake --build . --config {Realese}

@echo on
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
echo "Starting Build for all Projects with proposed changes"
echo "Shaderc builds started..." 
devenv "%CD%\shaderc.sln" /build "Debug|x64"
devenv "%CD%\shaderc.sln" /build "Release|x64"
echo "Shaderc builds finished..." 
echo "All builds completed."
cd ../

pause