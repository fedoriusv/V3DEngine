cmake_minimum_required(VERSION 2.8.9)

project(${GLEW_PLOJECT})

if(MINGW OR UNIX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-attributes")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} GL ")
endif()

include_directories( include )

add_definitions("-DGLEW_STATIC")

if(WIN32)
	file(GLOB GLEW_HEADERS include/GL/glew.h include/GL/wglew.h)
elseif(UNIX)
	file(GLOB GLEW_HEADERS include/GL/glew.h include/GL/glxew.h)
else()
	file(GLOB GLEW_HEADERS include/GL/glew.h)
endif()
file(GLOB GLEW_SOURCES src/glew.c)

source_group(include/GL FILES ${GLEW_HEADERS})
source_group(src FILES ${GLEW_SOURCES})

add_library(
	${GLEW_PLOJECT}
	STATIC
	${GLEW_SOURCES} ${GLEW_HEADERS}
	)
