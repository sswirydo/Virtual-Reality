#project setup
project(racing_game)

#set the minimal version of cmake and the minimal version of the c++ language
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 14)     
set(CMAKE_VERBOSE_MAKEFILE ON)

# CONFIGURE OPENGL
find_package(OpenGL REQUIRED)

add_compile_definitions(PATH_TO_PROJECT="${CMAKE_CURRENT_SOURCE_DIR}")

# CONFIGURE GLAD
add_library( glad STATIC libs/glad/src/glad.c)
set(GLAD_INCLUDE "libs/glad/include")
include_directories(libs/glad/include/)
target_include_directories(glad PUBLIC ${GLAD_INCLUDE})

# CONFIGURE GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(libs/glfw)
include_directories(libs/glfw/include/)

# CONFIGURE GLM
add_subdirectory(libs/glm) 
include_directories(libs/glm/)

#CONFIGURE STB
include_directories(libs/stb/)

# CONFIGURE ASSIMP
add_subdirectory(libs/assimp) 
include_directories(libs/assimp/include/)

# CONFIGURE BULLET
add_subdirectory(libs/bullet3)
include_directories(libs/bullet3/src)
LIST(APPEND BULLET_LIBS BulletDynamics BulletCollision LinearMath)
# add_definitions(-DBT_USE_DOUBLE_PRECISION)

# CONFIGURE IRRKLANG
include_directories(libs/irrKlang/include)
if (WIN32)
    link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/libs/irrKlang/lib/irrKlang.lib)
endif (WIN32)
if (UNIX)
    link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/libIrrKlang.so)
endif (UNIX)

# CONFIGURE OPEN AL
# add_subdirectory(libs/openal-soft)
# include_directories(libs/openal-soft/include)
# LIST(APPEND OTHER_LIBS OpenAL)

# CONFIGURE LIBSNDFILE
# add_subdirectory(libs/libsndfile)
# include_directories(libs/libsndfile/include)
# LIST(APPEND OTHER_LIBS sndfile)


## Out
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(code)
