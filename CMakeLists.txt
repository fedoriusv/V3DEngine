cmake_minimum_required(VERSION 3.0.0)

#Render SDK
option(BUILD_USE_VULKAN_SDK "Build use Vulkan SDK" ON)
option(BUILD_USE_OPENGL_SDK "Build use OpenGL" OFF)
option(BUILD_USE_DIRECTX_SDK "Build use DirectX SDK" OFF)

#Optional Libs
option(BUILD_USE_RESIL_LIB "Build use ResIL libraries" ON)
option(BUILD_USE_FREE_TYPE_LIB "Build use FreeType2 library" OFF)

#Tested projects
option(BUILD_TEST_PROJECT_0 "Build Test_CreateWindow" OFF)
option(BUILD_TEST_PROJECT_1 "Build Test_Font." OFF)
option(BUILD_TEST_PROJECT_2 "Build Test_RenderTarget." OFF)
option(BUILD_TEST_PROJECT_3 "Build Test_Billboard." OFF)
option(BUILD_TEST_PROJECT_4 "Build Test_Light." OFF)
option(BUILD_TEST_PROJECT_5 "Build Test_CreateContext." ON)
option(BUILD_TEST_PROJECT_6 "Build Test_Texture." OFF)
option(BUILD_TEST_PROJECT_7 "Build Test_ParticleSystem." OFF)
option(BUILD_TEST_PROJECT_8 "Build Test_Tessellation." OFF)

##############################################################

set(SOLUTION_NAME GameEngineV3D)
set(ENGINE_NAME V3DEngine)

#Require libs
set(TINYXML_PLOJECT tinyxml2)
set(GLEW_PLOJECT glew_static)

##############################################################

if (BUILD_TEST_PROJECT_0)
    set(TEST_PROJECT_0 Test_CreateWindow)
endif (BUILD_TEST_PROJECT_0)

if (BUILD_TEST_PROJECT_1)
    set(TEST_PROJECT_1 Test_Font)
endif (BUILD_TEST_PROJECT_1)

if (BUILD_TEST_PROJECT_2)
    set(TEST_PROJECT_2 Test_RenderTarget)
endif (BUILD_TEST_PROJECT_2)

if (BUILD_TEST_PROJECT_3)
    set(TEST_PROJECT_3 Test_Billboard)
endif (BUILD_TEST_PROJECT_3)

if (BUILD_TEST_PROJECT_4)
    set(TEST_PROJECT_4 Test_Light)
endif (BUILD_TEST_PROJECT_4)

if (BUILD_TEST_PROJECT_5)
    set(TEST_PROJECT_5 Test_CreateContext)
endif (BUILD_TEST_PROJECT_5)

if (BUILD_TEST_PROJECT_6)
   set(TEST_PROJECT_6 Test_Texture)
endif (BUILD_TEST_PROJECT_6)

if (BUILD_TEST_PROJECT_7)
    set(TEST_PROJECT_7 Test_ParticleSystem)
endif (BUILD_TEST_PROJECT_7)

if (BUILD_TEST_PROJECT_8)
    set(TEST_PROJECT_8 Test_Tessellation)
endif (BUILD_TEST_PROJECT_8)

##############################################################

if(COMPILER_MSVC)
    message("-- Configuring MSVC Compiler")
elseif(COMPILER_GCC)
    message("-- Configuring GCC Compiler")
elseif(COMPILER_CLANG)
    message("-- Configuring Clang Compiler")
else()
    message(FATAL_ERROR "Unknown compiler. Only COMPILER_MSVC|COMPILER_GCC|COMPILER_CLANG supported")
endif()

if(WIN32)
    message("-- Platform Windows")
    add_definitions(-D_WIN32 -D_CONSOLE -DUNICODE -D_UNICODE)
elseif(UNIX AND NOT APPLE)
    message("-- Platform Linux")
    add_definitions(-D_LINUX -DUNICODE -D_UNICODE)
elseif(APPLE)
    message("-- Platform MacOSX")
    add_definitions(-DMACOSX -DUNICODE -D_UNICODE)
else()
    message(FATAL_ERROR "Unknown platform. Only Platrorm Windows|Platrorm Linux|Platrorm MacOSX supported")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only support x64 Architecture")
endif()

##############################################################

project(${SOLUTION_NAME})

##############################################################

#Pathes
set(SOURCE_DIR source)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs)
set(TESTS_BIN_DIR ${CMAKE_BINARY_DIR}/tests)
set(ENGINE_PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(LIB_INCLUDE_DIRECTORY ${ENGINE_PROJECT_DIR}/${SOURCE_DIR}
    ${ENGINE_PROJECT_DIR}/${SOURCE_DIR}/ThirdParty
    ${ENGINE_PROJECT_DIR}/libs/glew/include;
    ${ENGINE_PROJECT_DIR}/libs/tinyxml2;
    )

if(BUILD_USE_FREE_TYPE_LIB)
    set(FREETYPE2_PROJECT freetype2)
    set(FREETYPE_DEFINE "-DUSED_FREETYPE=1")
    set(FREETYPE_INCLUDE_DIRECTORY ${ENGINE_PROJECT_DIR}/libs/freetype2/include)
else(BUILD_USE_FREE_TYPE_LIB)
    set(FREETYPE_DEFINE "-DUSED_FREETYPE=0")
endif(BUILD_USE_FREE_TYPE_LIB)

if(BUILD_USE_RESIL_LIB)
    set(RESIL_PROJECT ResIL)
    set(RESIL_DEFINE "-DUSED_DEVIL=1")
    set(RESIL_INCLUDE_DIRECTORY ${ENGINE_PROJECT_DIR}/libs/ResIL/include)
    set(RESIL_INCLUDE_SOURCE_DIRECTORY ${ENGINE_PROJECT_DIR}/libs/ResIL/src-IL/include)
else(BUILD_USE_RESIL_LIB)
    set(RESIL_DEFINE "-DUSED_DEVIL=0")
endif(BUILD_USE_RESIL_LIB)

if(BUILD_USE_VULKAN_SDK AND NOT APPLE)
    set(VULKAN_SDK_DEFINE "-DUSED_VULKAN_RENDER=1")
    #VK_SDK_PATH - must be added to system variables
    set(VULKAN_SDK_PATH $ENV{VK_SDK_PATH})
    set(VULKAN_SDK_INCLUDE_DIRECTORY ${VULKAN_SDK_PATH}/include)
    set(VULKAN_SDK_LIB_DIRECTORY ${VULKAN_SDK_PATH}/Bin)
    if (IS_DIRECTORY ${LIB_DIR}/spirv/shaderc AND IS_DIRECTORY ${LIB_DIR}/spirv/SPIRV-Cross)
    
        set(LIB_INCLUDE_DIRECTORY ${LIB_INCLUDE_DIRECTORY} ${LIB_DIR}/spirv/shaderc/libshaderc/include)
        set(LIB_INCLUDE_DIRECTORY ${LIB_INCLUDE_DIRECTORY} ${LIB_DIR}/spirv/SPIRV-Cross)
        
        set(SHADERC_LIB_FILE_DEBUG ${LIB_DIR}/spirv/shaderc/libshaderc/Debug/shaderc.lib)
        set(SHADERC_COMBINED_LIB_FILE_DEBUG ${LIB_DIR}/spirv/shaderc/libshaderc/Debug/shaderc_combined.lib)
        set(SHADERC_LIB_FILE_RELEASE ${LIB_DIR}/spirv/shaderc/libshaderc/Release/shaderc.lib)
        set(SHADERC_COMBINED_LIB_FILE_RELEASE ${LIB_DIR}/spirv/shaderc/libshaderc/Release/shaderc_combined.lib)
        
        set(SPIRV_CROSS_CPP_DEBUG ${LIB_DIR}/spirv/SPIRV-Cross/Debug/spirv-cross-cpp.lib)
        set(SPIRV_CROSS_CORE_DEBUG ${LIB_DIR}/spirv/SPIRV-Cross/Debug/spirv-cross-core.lib)
        set(SPIRV_CROSS_GLSL_DEBUG ${LIB_DIR}/spirv/SPIRV-Cross/Debug/spirv-cross-glsl.lib)
        set(SPIRV_CROSS_MSL_DEBUG ${LIB_DIR}/spirv/SPIRV-Cross/Debug/spirv-cross-msl.lib)  
        set(SPIRV_CROSS_CPP_RELEASE ${LIB_DIR}/spirv/SPIRV-Cross/Release/spirv-cross-cpp.lib)
        set(SPIRV_CROSS_CORE_RELEASE ${LIB_DIR}/spirv/SPIRV-Cross/Release/spirv-cross-core.lib)
        set(SPIRV_CROSS_GLSL_RELEASE ${LIB_DIR}/spirv/SPIRV-Cross/Release/spirv-cross-glsl.lib)
        set(SPIRV_CROSS_MSL_RELEASE ${LIB_DIR}/spirv/SPIRV-Cross/Release/spirv-cross-msl.lib)
  
        set(SPIRV_DEFINE "-DUSED_SPIRV=1")
    else()
        message(WARNING "spirv libs not found. Please run install script")
        set(SPIRV_DEFINE "-DUSED_SPIRV=0")
    endif()
else(BUILD_USE_VULKAN_SDK AND NOT APPLE)
    set(VULKAN_SDK_DEFINE "-DUSED_VULKAN_RENDER=0")
    set(SPIRV_DEFINE "-DUSED_SPIRV=0")
endif(BUILD_USE_VULKAN_SDK AND NOT APPLE)

if (BUILD_USE_DIRECTX_SDK AND WIN32)
    set(DIRECTX_SDK_DEFINE "-DUSED_D3D_RENDER=1")
else(BUILD_USE_DIRECTX_SDK AND WIN32)
    set(DIRECTX_SDK_DEFINE "-DUSED_D3D_RENDER=0")
endif(BUILD_USE_DIRECTX_SDK AND WIN32)

if (BUILD_USE_OPENGL_SDK)
    set(OPENGL_SDK_DEFINE "-DUSED_GL_RENDER=1")
else(BUILD_USE_OPENGL_SDK)
    set(OPENGL_SDK_DEFINE "-DUSED_GL_RENDER=0")
endif(BUILD_USE_OPENGL_SDK)

##############################################################

include_directories(BEFORE ${LIB_INCLUDE_DIRECTORY})

##############################################################

if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release;F3DDebug;F3DRelease" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /MP")
#   F3D Max Editor Debug
    set(CMAKE_CXX_FLAGS_F3DDEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -DDEBUG -DUSED_LOGGER=1 /Zi")
    set(CMAKE_C_FLAGS_F3DDEBUG ${CMAKE_C_FLAGS_DEBUG})
    set(CMAKE_EXE_LINKER_FLAGS_F3DDEBUG ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
    set(CMAKE_F3DDEBUG_POSTFIX "_d")
#   F3D Max Editor Release
    set(CMAKE_CXX_FLAGS_F3DRELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DUSED_LOGGER=0")
    set(CMAKE_C_FLAGS_F3DRELEASE ${CMAKE_C_FLAGS_RELEASE})
    set(CMAKE_EXE_LINKER_FLAGS_F3DRELEASE ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
#   Debug
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -DDEBUG -DUSED_LOGGER=1 ${RESIL_DEFINE} ${SPIRV_DEFINE} ${FREETYPE_DEFINE} ${VULKAN_SDK_DEFINE} ${DIRECTX_SDK_DEFINE} ${OPENGL_SDK_DEFINE}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -I${FREETYPE_INCLUDE_DIRECTORY}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -I${RESIL_INCLUDE_DIRECTORY}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -I${RESIL_INCLUDE_SOURCE_DIRECTORY}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -I${VULKAN_SDK_INCLUDE_DIRECTORY}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /MTd")
    set(CMAKE_DEBUG_POSTFIX "_d")
#   Release
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DUSED_LOGGER=0 ${RESIL_DEFINE} ${SPIRV_DEFINE} ${FREETYPE_DEFINE} ${VULKAN_SDK_DEFINE} ${DIRECTX_SDK_DEFINE} ${OPENGL_SDK_DEFINE}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -I${FREETYPE_INCLUDE_DIRECTORY}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -I${RESIL_INCLUDE_DIRECTORY}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -I${RESIL_INCLUDE_SOURCE_DIRECTORY}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -I${VULKAN_SDK_INCLUDE_DIRECTORY}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
elseif(APPLE)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -x objective-c++" )
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -DDEBUG -DUSED_LOGGER=1 ${RESIL_DEFINE} ${OPENGL_SDK_DEFINE}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DUSED_LOGGER=0 ${RESIL_DEFINE} ${OPENGL_SDK_DEFINE}")
#   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_DEBUG_POSTFIX "_d")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -DDEBUG -DUSED_LOGGER=1 ${RESIL_DEFINE} ${SPIRV_DEFINE} ${OPENGL_SDK_DEFINE} ${VULKAN_SDK_DEFINE}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DUSED_LOGGER=0 ${RESIL_DEFINE} -DUSED_GL_RENDER=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lX11 -lGL" )
    set(CMAKE_DEBUG_POSTFIX "_d")
endif()

##############################################################

if(WIN32)
    if(CMAKE_CL_64)
        set(ARCH_DIR "x64")
    else(CMAKE_CL_64)
        set(ARCH_DIR "x86")
    endif(CMAKE_CL_64)
    
    if(BUILD_USE_RESIL_LIB)
        set(RESIL_LIBPATH libs/ResIL/win32/${ARCH_DIR}-msvcrt)
        set(RESIL_PROJECT ../../${RESIL_LIBPATH}/ResIL)
        set(RESIL_BIN ${LIB_DIR}/ResIL/win32/${ARCH_DIR}-msvcrt)
    endif(BUILD_USE_RESIL_LIB)
endif(WIN32)

##############################################################

#Platform Source
    set(PLATFORM_HEADERS
        ${SOURCE_DIR}/platform/Platform.h
        ${SOURCE_DIR}/platform/Window.h
        )
        
    set(PLATFORM_SOURCES
        ${SOURCE_DIR}/platform/Platform.cpp	
        ${SOURCE_DIR}/platform/Window.cpp
        )

if(WIN32)
    set(PLATFORM_HEADERS ${PLATFORM_HEADERS}
        ${SOURCE_DIR}/platform/WindowWinApi.h
        )
    
    set(PLATFORM_SOURCES ${PLATFORM_SOURCES}
        ${SOURCE_DIR}/platform/WindowWinApi.cpp
        )
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_HEADERS ${PLATFORM_HEADERS}
        ${SOURCE_DIR}/platform/WindowLinux.h
        )
    
    set(PLATFORM_SOURCES ${PLATFORM_SOURCES}
        ${SOURCE_DIR}/platform/WindowLinux.cpp
        )
elseif(APPLE)
    set(PLATFORM_HEADERS ${PLATFORM_HEADERS}
        ${SOURCE_DIR}/platform/WindowMacOSX.h
        )
    
    set(PLATFORM_SOURCES ${PLATFORM_SOURCES}
        ${SOURCE_DIR}/platform/WindowMacOSX.mm
        )
endif()

##############################################################

#Core Source
file(GLOB CORE_HEADERS ${SOURCE_DIR}/core/*.h)
file(GLOB CORE_INLINES ${SOURCE_DIR}/core/*.inl)
file(GLOB CORE_SOURCES ${SOURCE_DIR}/core/*.cpp)

#Renderer Source
file(GLOB RENDERER_HEADERS ${SOURCE_DIR}/renderer/*.h)
file(GLOB RENDERER_SOURCES ${SOURCE_DIR}/renderer/*.cpp)

#Resources Source
file(GLOB RESOURCES_HEADERS ${SOURCE_DIR}/resources/*.h)
file(GLOB RESOURCES_SOURCES ${SOURCE_DIR}/resources/*.cpp)

#Decoders Source
file(GLOB DECODERS_HEADERS ${SOURCE_DIR}/decoders/*.h)
file(GLOB DECODERS_SOURCES ${SOURCE_DIR}/decoders/*.cpp)

#GL Source
file(GLOB GL_HEADERS ${SOURCE_DIR}/renderer/GL/*.h)
file(GLOB GL_SOURCES ${SOURCE_DIR}/renderer/GL/*.cpp)

#D3D Source
file(GLOB D3D_HEADERS ${SOURCE_DIR}/renderer/D3D/*.h)
file(GLOB D3D_SOURCES ${SOURCE_DIR}/renderer/D3D/*.cpp)

#VK Source
file(GLOB VK_HEADERS ${SOURCE_DIR}/renderer/VK/*.h)
file(GLOB VK_SOURCES ${SOURCE_DIR}/renderer/VK/*.cpp)

#NULL Source
file(GLOB NULL_HEADERS ${SOURCE_DIR}/renderer/NULL/*.h)
file(GLOB NULL_SOURCES ${SOURCE_DIR}/renderer/NULL/*.cpp)

#Context Source
file(GLOB CONTEXT_HEADERS ${SOURCE_DIR}/context/*.h)
file(GLOB CONTEXT_SOURCES ${SOURCE_DIR}/context/*.cpp)

#Scene Source
file(GLOB SCENE_HEADERS ${SOURCE_DIR}/scene/*.h)
file(GLOB SCENE_SOURCES ${SOURCE_DIR}/scene/*.cpp)

#Stream Source
file(GLOB STREAM_HEADERS ${SOURCE_DIR}/stream/*.h)
file(GLOB STREAM_INLINES ${SOURCE_DIR}/stream/*.inl)
file(GLOB STREAM_SOURCES ${SOURCE_DIR}/stream/*.cpp)

#Utils Source
file(GLOB UTILS_HEADERS ${SOURCE_DIR}/utils/*.h)
file(GLOB UTILS_SOURCES ${SOURCE_DIR}/utils/*.cpp)

#Audio Source
file(GLOB AUDIO_HEADERS ${SOURCE_DIR}/audio/*.h)
file(GLOB AUDIO_SOURCES ${SOURCE_DIR}/audio/*.cpp)

#Phisics Source
file(GLOB PHISICS_HEADERS ${SOURCE_DIR}/phisics/*.h)
file(GLOB PHISICS_SOURCES ${SOURCE_DIR}/phisics/*.cpp)

#Event Source
file(GLOB EVENT_HEADERS ${SOURCE_DIR}/event/*.h)
file(GLOB EVENT_SOURCES ${SOURCE_DIR}/event/*.cpp)

#Main Source
file(GLOB MAIN_HEADERS ${SOURCE_DIR}/*.h)
file(GLOB MAIN_SOURCES ${SOURCE_DIR}/*.cpp)

##############################################################

#Filters
source_group(platform         FILES ${PLATFORM_HEADERS}    ${PLATFORM_SOURCES})
source_group(core             FILES ${CORE_HEADERS}        ${CORE_SOURCES}          ${CORE_INLINES})
source_group(renderer         FILES ${RENDERER_HEADERS}    ${RENDERER_SOURCES})
source_group(resources        FILES ${RESOURCES_HEADERS}   ${RESOURCES_SOURCES})
source_group(decoders         FILES ${DECODERS_HEADERS}    ${DECODERS_SOURCES})
source_group(renderer/gl      FILES ${GL_HEADERS}          ${GL_SOURCES})
source_group(renderer/d3d     FILES ${D3D_HEADERS}         ${D3D_SOURCES})
source_group(renderer/vk      FILES ${VK_HEADERS}          ${VK_SOURCES})
source_group(renderer/null    FILES ${NULL_HEADERS}        ${NULL_SOURCES})
source_group(context          FILES ${CONTEXT_HEADERS}     ${CONTEXT_SOURCES})
source_group(stream           FILES ${STREAM_HEADERS}      ${STREAM_SOURCES}        ${STREAM_INLINES})
source_group(scene            FILES ${SCENE_HEADERS}       ${SCENE_SOURCES})
source_group(utils            FILES ${UTILS_HEADERS}       ${UTILS_SOURCES})
source_group(event            FILES ${EVENT_HEADERS}       ${EVENT_SOURCES})
source_group(audio            FILES ${AUDIO_HEADERS}       ${AUDIO_SOURCES})
source_group(phisics          FILES ${PHISICS_HEADERS}     ${PHISICS_SOURCES})
source_group(""               FILES ${MAIN_HEADERS}        ${MAIN_SOURCES})

##############################################################

add_library(
    ${ENGINE_NAME} STATIC
    ${PLATFORM_HEADERS}    ${PLATFORM_SOURCES}
    ${CORE_HEADERS}        ${CORE_SOURCES}
    ${RENDERER_HEADERS}    ${RENDERER_SOURCES}
    ${DECODERS_HEADERS}    ${DECODERS_SOURCES}
    ${RESOURCES_HEADERS}   ${RESOURCES_SOURCES}
    ${GL_HEADERS}          ${GL_SOURCES}
    ${D3D_HEADERS}         ${D3D_SOURCES}
    ${VK_HEADERS}          ${VK_SOURCES}
    ${NULL_HEADERS}        ${NULL_SOURCES}
    ${CONTEXT_HEADERS}     ${CONTEXT_SOURCES}
    ${STREAM_HEADERS}      ${STREAM_SOURCES}
    ${SCENE_HEADERS}       ${SCENE_SOURCES}
    ${UTILS_HEADERS}       ${UTILS_SOURCES}
    ${EVENT_HEADERS}       ${EVENT_SOURCES}
    ${MAIN_HEADERS}        ${MAIN_SOURCES}
    )

##############################################################

#link_libraries
set(BUILD_LINK_LIBRARIES ${TINYXML_PLOJECT})
#${GLEW_PLOJECT} included from source files
#${RESIL_LIB} included from source files

target_link_libraries(${ENGINE_NAME} ${BUILD_LINK_LIBRARIES})
if(BUILD_USE_FREE_TYPE_LIB)
    target_link_libraries(${ENGINE_NAME} ${FREETYPE2_PROJECT})
endif(BUILD_USE_FREE_TYPE_LIB)

if(BUILD_USE_VULKAN_SDK)
    target_link_libraries(${ENGINE_NAME} optimized ${SHADERC_LIB_FILE_RELEASE}          debug ${SHADERC_LIB_FILE_DEBUG})
    target_link_libraries(${ENGINE_NAME} optimized ${SHADERC_COMBINED_LIB_FILE_RELEASE} debug ${SHADERC_COMBINED_LIB_FILE_DEBUG})
    target_link_libraries(${ENGINE_NAME} optimized ${SPIRV_CROSS_CPP_RELEASE}           debug ${SPIRV_CROSS_CPP_DEBUG})
    target_link_libraries(${ENGINE_NAME} optimized ${SPIRV_CROSS_CORE_RELEASE}          debug ${SPIRV_CROSS_CORE_DEBUG})
    target_link_libraries(${ENGINE_NAME} optimized ${SPIRV_CROSS_GLSL_RELEASE}          debug ${SPIRV_CROSS_GLSL_DEBUG})
    target_link_libraries(${ENGINE_NAME} optimized ${SPIRV_CROSS_MSL_RELEASE}           debug ${SPIRV_CROSS_MSL_DEBUG})
endif(BUILD_USE_VULKAN_SDK)

##############################################################

#LINK_DERICTORIES
set(LINK_DERICTORIES ${PROJECT_BINARY_DIR}/libs/glew)

if(BUILD_USE_RESIL_LIB)
    set(LINK_DERICTORIES ${LINK_DERICTORIES} ${RESIL_LIBPATH})
endif(BUILD_USE_RESIL_LIB)

if(BUILD_USE_VULKAN_SDK)
    set(LINK_DERICTORIES ${LINK_DERICTORIES} ${VULKAN_SDK_LIB_DIRECTORY})
endif(BUILD_USE_VULKAN_SDK)

link_directories(${ENGINE_NAME} ${LINK_DERICTORIES})

##############################################################

#Additional Libs
message("-- Include Source Lib TINYXML2")
add_subdirectory(libs/tinyxml2)

if (BUILD_USE_OPENGL_SDK)
    message("-- Include Source Lib GLEW")
    add_subdirectory(libs/glew)
endif(BUILD_USE_OPENGL_SDK)

if(BUILD_USE_FREE_TYPE_LIB)
    message("-- Include Source Lib FREETYPE2")
    add_subdirectory(libs/freetype2)
endif(BUILD_USE_FREE_TYPE_LIB)

if(NOT WIN32 AND BUILD_USE_RESIL_LIB)
    message("-- Include Source Lib ResIL")
    add_subdirectory(libs/ResIL)
endif(NOT WIN32 AND BUILD_USE_RESIL_LIB)

##############################################################

if (BUILD_TEST_PROJECT_0)
    message("-- Create project ${TEST_PROJECT_0}")
    add_subdirectory(tests/${TEST_PROJECT_0})
endif (BUILD_TEST_PROJECT_0)

if (BUILD_TEST_PROJECT_1)
    message("-- Create project ${TEST_PROJECT_1}")
    add_subdirectory(tests/${TEST_PROJECT_1})
endif (BUILD_TEST_PROJECT_1)

if (BUILD_TEST_PROJECT_2)
    message("-- Create project ${TEST_PROJECT_2}")
    add_subdirectory(tests/${TEST_PROJECT_2})
endif (BUILD_TEST_PROJECT_2)

if (BUILD_TEST_PROJECT_3)
    message("-- Create project ${TEST_PROJECT_3}")
    add_subdirectory(tests/${TEST_PROJECT_3})
endif (BUILD_TEST_PROJECT_3)

if (BUILD_TEST_PROJECT_4)
    message("-- Create project ${TEST_PROJECT_4}")
    add_subdirectory(tests/${TEST_PROJECT_4})
endif (BUILD_TEST_PROJECT_4)

if (BUILD_TEST_PROJECT_5)
    message("-- Create project ${TEST_PROJECT_5}")
    add_subdirectory(tests/${TEST_PROJECT_5})
endif (BUILD_TEST_PROJECT_5)

if (BUILD_TEST_PROJECT_6)
    message("-- Create project ${TEST_PROJECT_6}")
    add_subdirectory(tests/${TEST_PROJECT_6})
endif (BUILD_TEST_PROJECT_6)

if (BUILD_TEST_PROJECT_7)
    message("-- Create project ${TEST_PROJECT_7}")
    add_subdirectory(tests/${TEST_PROJECT_7})
endif (BUILD_TEST_PROJECT_7)

if (BUILD_TEST_PROJECT_8)
    message("-- Create project ${TEST_PROJECT_8}")
    add_subdirectory(tests/${TEST_PROJECT_8})
endif (BUILD_TEST_PROJECT_8)

##############################################################

#Dependencies
set(BUILD_DEPENDENCIES ${TINYXML_PLOJECT})

if (BUILD_USE_OPENGL_SDK)
    set(BUILD_DEPENDENCIES ${BUILD_DEPENDENCIES} ${GLEW_PLOJECT})
endif (BUILD_USE_OPENGL_SDK)

if(BUILD_USE_FREE_TYPE_LIB)
    set(BUILD_DEPENDENCIES ${BUILD_DEPENDENCIES} ${FREETYPE2_PROJECT})
endif(BUILD_USE_FREE_TYPE_LIB)

if(NOT WIN32 AND BUILD_USE_RESIL_LIB)
    set(BUILD_DEPENDENCIES ${BUILD_DEPENDENCIES} ${RESIL_LIB})
endif(NOT WIN32 AND BUILD_USE_RESIL_LIB)

add_dependencies(${ENGINE_NAME} ${BUILD_DEPENDENCIES})

##############################################################