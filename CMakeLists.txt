cmake_minimum_required(VERSION 2.8.9)

set(SOLUTION_NAME Fedoria3D)
set(ENGINE_NAME Fedoria3DEngine)
set(GLEW_PLOJECT glew_static)
set(TINYXML_PLOJECT tinyxml2static)
set(TEST_PROJECT_0 CreateWindow)

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
	message("-- Platrorm Win32")
	add_definitions(-D_WIN32 -D_CONSOLE -DUNICODE -D_UNICODE)
elseif(UNIX AND NOT APPLE)
	message("-- Platrorm Linux")
	add_definitions(-D_LINUX)
elseif(APPLE)
	message("-- Platrorm MacOSX")
	add_definitions(-DMACOSX)
else()
	message(FATAL_ERROR "Unknown platform. Only Platrorm Win32|Platrorm Linux|Platrorm MacOSX supported")
endif()

project(${SOLUTION_NAME})

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
elseif(UNIX OR APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lX11 -lGL" )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa")
endif()

################################
#Source Dir
set(SOURCE_DIR source)

set(MAIN_FILE 
	${SOURCE_DIR}/Object.cpp
	${SOURCE_DIR}/Object.h
 )

#Filters
################################
#Platform Source
if(WIN32) 
	set(PLATFORM_HEADERS
		${SOURCE_DIR}/platform/Platform.h
		${SOURCE_DIR}/platform/Window.h
		${SOURCE_DIR}/platform/WindowWin32.h
	)
	
	set(PLATFORM_SOURCES
		${SOURCE_DIR}/platform/Platform.cpp
		${SOURCE_DIR}/platform/Window.cpp
		${SOURCE_DIR}/platform/WindowWin32.cpp
	)

elseif(UNIX AND NOT APPLE)
	set(PLATFORM_HEADERS
		${SOURCE_DIR}/platform/Platform.h
		${SOURCE_DIR}/platform/PlatformManager.h
		${SOURCE_DIR}/platform/PlatformLinux.h
	)
	
	set(PLATFORM_SOURCES
		${SOURCE_DIR}/platform/Platform.cpp
		${SOURCE_DIR}/platform/PlatformManager.cpp
		${SOURCE_DIR}/platform/PlatformLinux.cpp
	)

elseif(APPLE)
	set(PLATFORM_HEADERS
		${SOURCE_DIR}/platform/Platform.h
		${SOURCE_DIR}/platform/Window.h
		${SOURCE_DIR}/platform/WindowMacOSX.h
	)
	
	set(PLATFORM_SOURCES
		${SOURCE_DIR}/platform/Platform.cpp
		${SOURCE_DIR}/platform/Window.cpp
		${SOURCE_DIR}/platform/WindowMacOSX.mm
	)
endif()

#Core Source
file(GLOB CORE_HEADERS ${SOURCE_DIR}/core/*.h)
file(GLOB CORE_SOURCES ${SOURCE_DIR}/core/*.cpp)

#Renderer Source
file(GLOB RENDERER_HEADERS ${SOURCE_DIR}/renderer/*.h)
file(GLOB RENDERER_SOURCES ${SOURCE_DIR}/renderer/*.cpp)

#GL Source
file(GLOB GL_HEADERS ${SOURCE_DIR}/*/GL/*.h)
file(GLOB GL_SOURCES ${SOURCE_DIR}/*/GL/*.cpp)

#Context Source
file(GLOB CONTEXT_HEADERS ${SOURCE_DIR}/context/*.h)
file(GLOB CONTEXT_SOURCES ${SOURCE_DIR}/context/*.cpp)

#Scene Source
file(GLOB SCENE_HEADERS ${SOURCE_DIR}/scene/*.h)
file(GLOB SCENE_SOURCES ${SOURCE_DIR}/scene/*.cpp)

#Stream Source
file(GLOB STREAM_HEADERS ${SOURCE_DIR}/stream/*.h)
file(GLOB STREAM_SOURCES ${SOURCE_DIR}/stream/*.cpp)

#Utils Source
file(GLOB UTILS_HEADERS ${SOURCE_DIR}/utils/*.h)
file(GLOB UTILS_SOURCES ${SOURCE_DIR}/utils/*.cpp)

#Event Source
file(GLOB EVENT_HEADERS ${SOURCE_DIR}/event/*.h)
file(GLOB EVENT_SOURCES ${SOURCE_DIR}/event/*.cpp)

#Main Source
file(GLOB MAIN_HEADERS ${SOURCE_DIR}/*.h)
file(GLOB MAIN_SOURCES ${SOURCE_DIR}/*.cpp)

source_group(platform         FILES ${PLATFORM_HEADERS}    ${PLATFORM_SOURCES})
source_group(core             FILES ${CORE_HEADERS}        ${CORE_SOURCES})
source_group(renderer         FILES ${RENDERER_HEADERS}    ${RENDERER_SOURCES})
source_group(gl               FILES ${GL_HEADERS}          ${GL_SOURCES})
source_group(context          FILES ${CONTEXT_HEADERS}     ${CONTEXT_SOURCES})
source_group(stream           FILES ${STREAM_HEADERS}      ${STREAM_SOURCES})
source_group(scene            FILES ${SCENE_HEADERS}       ${SCENE_SOURCES})
source_group(utils            FILES ${UTILS_HEADERS}       ${UTILS_SOURCES})
source_group(event            FILES ${EVENT_HEADERS}       ${EVENT_SOURCES})
source_group(""               FILES ${MAIN_HEADERS}        ${MAIN_SOURCES})

set (ENGINE_PROJECT_DIR ${PROJECT_SOURCE_DIR})

include_directories(
	${SOURCE_DIR}
	libs/glew/include
	libs/tinyxml2
	)

add_library(
	${ENGINE_NAME}
	STATIC
	${PLATFORM_HEADERS}    ${PLATFORM_SOURCES}
	${CORE_HEADERS}        ${CORE_SOURCES}
	${RENDERER_HEADERS}    ${RENDERER_SOURCES}
	${GL_HEADERS}          ${GL_SOURCES}
	${CONTEXT_HEADERS}     ${CONTEXT_SOURCES}
	${STREAM_HEADERS}      ${STREAM_SOURCES}
	${SCENE_HEADERS}       ${SCENE_SOURCES}
	${UTILS_HEADERS}       ${UTILS_SOURCES}
	${EVENT_HEADERS}       ${EVENT_SOURCES}
	${MAIN_HEADERS}        ${MAIN_SOURCES}
	) 

target_link_libraries(${ENGINE_NAME} 
	${GLEW_PLOJECT}
	${TINYXML_PLOJECT}
	)

################################
#Additional Libs
add_subdirectory(libs/glew)
add_subdirectory(libs/tinyxml2)
add_subdirectory(tests/${TEST_PROJECT_0})

add_dependencies(${ENGINE_NAME} 
	${GLEW_PLOJECT}
	${TINYXML_PLOJECT}
	)




