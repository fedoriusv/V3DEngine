cmake_minimum_required(VERSION 2.8.9)

set (CURRENT_PROJECT ${TEST_PROJECT_5})
project(${CURRENT_PROJECT})

file(GLOB TEST_HEADERS *.h)
file(GLOB TEST_SOURCES *.cpp)

set(MAIN_FILE CreateWindow.cpp)

source_group(include FILES ${TEST_HEADERS})
source_group(source  FILES ${TEST_SOURCES})

include_directories(source)

if(WIN32)
	link_directories(${ENGINE_PROJECT_DIR}/project/win32/)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
elseif(UNIX AND NOT APPLE)
	link_directories(${ENGINE_PROJECT_DIR}/project/linux-qt/)
elseif(APPLE)
    link_directories(${ENGINE_PROJECT_DIR}/project/xcode/)
endif()

add_executable(
	${CURRENT_PROJECT}
    WIN32 MACOSX_BUNDLE
	${MAIN_FILE}
	${TEST_HEADERS}
	${TEST_SOURCES}
	)

set_target_properties(${CURRENT_PROJECT} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Test_CreateWindow")

if(WIN32)
	set(EXTERNAL_LIBS 
		#OpenGL32.lib
		#debug
		#debug ${ENGINE_NAME}_d.lib
		#debug ${GLEW_PLOJECT}_d.lib
		#release
		#optimized ${ENGINE_NAME}.lib
		#optimized ${GLEW_PLOJECT}.lib
	)
    
	if(BUILD_USE_RESIL_LIB)
        file(COPY ${RESIL_BIN}/ResIL.dll DESTINATION ${TESTS_BIN_DIR}/${CURRENT_PROJECT}/Debug)
        file(COPY ${RESIL_BIN}/ResIL.dll DESTINATION ${TESTS_BIN_DIR}/${CURRENT_PROJECT}/Release)
	endif(BUILD_USE_RESIL_LIB)
elseif(UNIX AND NOT APPLE)
	find_package(X11)
	set(EXTERNAL_LIBS
		libX11.so
		libGL.so
		libGLU.so
	)
	#find_library(X11 NAMES X11 x11 PATHS /usr/X11/include/X11/ /usr/X11/lib)
elseif(APPLE)
    include_directories(/System/Library/Frameworks)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    
    set(EXTERNAL_LIBS
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
    )
endif()

target_link_libraries(
	${CURRENT_PROJECT}
	${EXTERNAL_LIBS}
	${ENGINE_NAME}
	)

add_dependencies(${CURRENT_PROJECT} ${ENGINE_NAME})
