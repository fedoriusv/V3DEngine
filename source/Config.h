// Valera3D Engine (c) 2014 Bogdan Klochko / Fedor Nekrasov

#ifndef _V3D_CONFIG_H_
#define _V3D_CONFIG_H_

//Version 2.0.0
#define VERSION_MAJOR       2
#define VERSION_MINOR       0
#define VERSION_REVISION    0

//Config
/////////////////////////////////////////////////////////////////////////////////////////////////////

#define SHADER_PARSER 100
#define DIRECTINPUT_VERSION 0x0800

#define USE_LOGGER 1
#define USE_DEVIL

//Compiler
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) && (_MSC_VER < 1700)
#   error "Only Microsoft Visual Studio 11.0 and later are supported."
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



//Video Driver
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_PLATFORM_WIN_)
#   define _OPENGL_DRIVER_
#   define _DIRECT3D_DRIVER_

#   define _DIRECTINPUT_ 1
#endif

#if defined (_PLATFORM_LINUX_)
#   define _OPENGL_DRIVER_
#endif

#if defined (_PLATFORM_MACOSX_)
#   define _OPENGL_DRIVER_
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _OPENGL_DRIVER_
#   define OPENGL_VERSION_MAJOR 3
#   define OPENGL_VERSION_MINOR 3

#   define GLEW_STATIC
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //_V3D_CONFIG_H_
