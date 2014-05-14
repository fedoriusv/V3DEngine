// Fedoria3D Engine (c) 2014 Fedor Nekrasov

#ifndef _F3D_CONFIG_H_
#define _F3D_CONFIG_H_

//Version 2.0.0
#define VERSION_MAJOR		2
#define VERSION_MINOR		0
#define VERSION_REVISION	0

//Config
/////////////////////////////////////////////////////////////////////////////////////////////////////

#define USE_LOGGER 1


//Compiler
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) && (_MSC_VER < 1700)
#	error "Only Microsoft Visual Studio 11.0 and later are supported."
#endif

//PLatform
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#	define _PLATFORM_WIN_
#endif

#if defined(__APPLE__) || defined(MACOSX)
#	define _PLATFORM_MACOSX_
#endif

#if defined(LINUX) || defined(_LINUX)
#	define _PLATFORM_LINUX_
#endif



//Video Driver
/////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_PLATFORM_WIN_)
#	define _OPENGL_DRIVER_
#	define _DIRECT3D_DRIVER_
#endif

#if defined (_PLATFORM_LINUX_)
#	define _OPENGL_DRIVER_
#endif

#if defined (_PLATFORM_MACOSX_)
#	define _OPENGL_DRIVER_
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //_F3D_FEDORIA_CONFIG_H_
