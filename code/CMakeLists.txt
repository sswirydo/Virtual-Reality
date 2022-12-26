add_library(
    Shader
    sources/Shader.cpp
    headers/Shader.hpp
)
target_include_directories(Shader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS Shader)

add_library(
  Physics
  sources/Physics.cpp
  headers/Physics.hpp
)
target_include_directories(Physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS Physics)

add_library(
    Texture
    sources/Texture.cpp
    headers/Texture.hpp
)
target_include_directories(Texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS Texture)

add_library(
    Game
    sources/Game.cpp
    headers/Game.hpp
)
target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS Game)

add_library(
    Object
    sources/Object.cpp
    headers/Object.hpp
)
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS Object)

add_library(
    Vertices STATIC
    sources/Vertices.cpp
    headers/Vertices.hpp
)
target_include_directories(Vertices PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS Vertices)

add_library(
    Mesh
    sources/Mesh.cpp
    headers/Mesh.hpp
)
target_include_directories(Mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS Mesh)

add_library(
    LightSource
    sources/LightSource.cpp
    headers/LightSource.hpp
)
target_include_directories(LightSource PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS LightSource)

add_library(
    Car
    sources/Car.cpp
    headers/Car.hpp
)
target_link_libraries(Car Mesh)
LIST(APPEND PROJECT_LIBS Car)

add_library(
    Model
    sources/Model.cpp
    headers/Model.hpp
)
target_link_libraries(Model Mesh)
target_include_directories(Model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
LIST(APPEND PROJECT_LIBS Model)

add_library(
    stb_image STATIC
    sources/stb_image.cpp
    ../libs/stb/stb_image.h
)
set(STB_INCLUDE "../libs/stb")
target_include_directories(stb_image PUBLIC ${STB_INCLUDE})

add_subdirectory(sources)
