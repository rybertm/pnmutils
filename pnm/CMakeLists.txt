cmake_minimum_required(VERSION 3.12.2)
project(pnm)

#Bring the headers, such as Student.h into the project
include_directories(include)

#add source files to project
file(GLOB SOURCES src/*.cpp)

add_executable(pnm ${SOURCES})
