// V3D Engine (c) 2016 Fred Nekrasov

#ifndef _V3D_CONFIG_H_
#define _V3D_CONFIG_H_

//Version 2.0.0
#define VERSION_MAJOR       2
#define VERSION_MINOR       0
#define VERSION_REVISION    0

//Config
/////////////////////////////////////////////////////////////////////////////////////////////////////

#define SHADER_PARSER 100
#define F3D_MODEL_LOADER_VERSION 1.0f
#define DIRECTINPUT_VERSION 0x0800

#if USED_LOGGER
#   define USE_LOGGER
#   define USE_DEBUG_LOG
#   define HIGHLIGHTING_LOGS 1
#endif //USED_LOGGER

#if USED_DEVIL
#   define USE_DEVIL
#endif //USED_DEVIL

//Compiler
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) && (_MSC_VER < 1900)
#   error "Only Microsoft Visual Studio 14.0 and later are supported."
#endif

//PLatform
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#   define _PLATFORM_WIN_
#endif

#if defined(__APPLE__) || defined(MACOSX)
#   define _PLATFORM_MACOSX_
#endif

#if defined(LINUX) || defined(_LINUX)
#   define _PLATFORM_LINUX_
#endif


//Render
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_PLATFORM_WIN_)
#if USED_GL_RENDER
#   define _OPENGL_RENDER_
#endif //USED_GL_RENDER

#if USED_D3D_RENDER
#   define _DIRECT3D_RENDER_
#endif //USED_D3D_RENDER

#if USED_VULKAN_RENDER
#   define _VULKAN_RENDER_
#endif //USED_VULKAN_RENDER

#define _DIRECTINPUT_ 1

#endif //_PLATFORM_WIN_

#if defined (_PLATFORM_LINUX_)
#if USED_GL_RENDER
#   define _OPENGL_RENDER_
#endif //USED_GL_RENDER
#endif //_PLATFORM_LINUX_

#if defined (_PLATFORM_MACOSX_)
#if USED_GL_RENDER
#   define _OPENGL_RENDER_
#endif //USED_GL_RENDER
#endif //_PLATFORM_MACOSX_

//Render Version
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _OPENGL_RENDER_
#   define OPENGL_VERSION_MAJOR 4
#   define OPENGL_VERSION_MINOR 4

#   define GLEW_STATIC
#   define _DEBUG_GL
#   define _DEBUG_GL_ERROR_ASSERT 1
#endif //_OPENGL_RENDER_

#ifdef _DIRECT3D_RENDER_
#   define DIRECTD3D_VERSION_MAJOR 11
#   define DIRECTD3D_VERSION_MINOR 0
#endif //_DIRECT3D_RENDER_

#ifdef _VULKAN_RENDER_
//
#endif //_DIRECT3D_RENDER_

/////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //_V3D_CONFIG_H_
