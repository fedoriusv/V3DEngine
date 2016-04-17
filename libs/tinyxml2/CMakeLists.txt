cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

project(${TINYXML_PLOJECT})
include(GNUInstallDirs)
#enable_testing()

#CMAKE_BUILD_TOOL

################################
# set lib version here

set(GENERIC_LIB_VERSION "3.0.0")
set(GENERIC_LIB_SOVERSION "3")


################################
# Add common source 

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/.")

################################
# Add custom target to copy all data

################################
# Add definitions

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

################################
# Add targets
option(BUILD_SHARED_LIBS "build shared or static libraries" OFF)
add_library(${TINYXML_PLOJECT} tinyxml2.cpp tinyxml2.h)
set_target_properties(${TINYXML_PLOJECT} PROPERTIES
        COMPILE_DEFINITIONS "TINYXML2_EXPORT"
    VERSION "${GENERIC_LIB_VERSION}"
    SOVERSION "${GENERIC_LIB_SOVERSION}")
