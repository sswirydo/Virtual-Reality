#project setup
project(VR-Project)

#set the minimal version of cmake and the minimal version of the c++ language
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)     
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(OpenGL REQUIRED)

#for glad library
add_library( glad STATIC libs/glad/src/glad.c)
set(GLAD_INCLUDE "libs/glad/include")
target_include_directories(glad PUBLIC ${GLAD_INCLUDE})

#for GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libs/glfw)


include_directories(libs/glad/include/
                    libs/glfw/include/
                    libs/glm/
                    libs/stb/)

## GLM
add_subdirectory(libs/glm) 

## Out
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(code)
