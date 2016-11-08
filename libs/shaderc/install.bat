@echo off
git clone https://github.com/google/shaderc

cd shaderc/third_party
REM  git clone https://github.com/google/googletest.git
git clone https://github.com/google/glslang.git
git clone https://github.com/KhronosGroup/SPIRV-Tools.git spirv-tools
git clone https://github.com/KhronosGroup/SPIRV-Headers.git spirv-tools/external/spirv-headers
cd ../

cmake -DSHADERC_SKIP_TESTS=ON -G"Visual Studio 14 Win64"
REM cmake --build . --config {Debug}
REM cmake --build . --config {Realese}

@echo on
call vcvarsall.bat x64
echo "Starting Build for all Projects with proposed changes"
echo "Shaderc builds started..." 
devenv "%CD%\shaderc.sln" /build "Debug|x64"
devenv "%CD%\shaderc.sln" /build "Release|x64"
echo "Shaderc builds finished..." 
echo "All builds completed."
cd ../

pause