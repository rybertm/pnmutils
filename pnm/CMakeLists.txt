cmake_minimum_required(VERSION 3.5)
project(pnmutils)

set(CMAKE_DEBUG_POSTFIX "d")

option(BUILD_TEST "Build test demo" OFF)
option(SHARED_LIB "Build pnm shared library." OFF)
option(STATIC_LIB "Build pnm static library." ON)

#Bring the headers into the project
include_directories(include)

#add source files to project
set(SOURCES src/pnmutils.cpp)

if(WIN32)
	set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/freeglut" CACHE STRING "freeglut path")
else()
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
endif()

# GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
if(NOT GLUT_FOUND)
	message(ERROR "GLUT not found!")
endif(NOT GLUT_FOUND)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
	message(ERROR "OPENGL not found!")
endif(NOT OPENGL_FOUND)

#	Build Libraries
if(SHARED_LIB)
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
	target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
endif()
if(STATIC_LIB)
	add_library(${PROJECT_NAME}_static STATIC ${SOURCES})
	target_link_libraries(${PROJECT_NAME}_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
endif()
#	Build Test
if(BUILD_TEST)
	add_executable(${PROJECT_NAME}_test ${SOURCES} src/test.cpp)
	target_link_libraries(${PROJECT_NAME}_test ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
endif()
