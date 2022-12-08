add_library(
    Shader
    sources/Shader.cpp
    headers/Shader.hpp
    )
target_include_directories(Shader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
    Texture
    sources/Texture.cpp
    headers/Texture.hpp
)
target_include_directories(Texture PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
    Game
    sources/Game.cpp
    headers/Game.hpp
)
target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
    Object
    sources/Object.cpp
    headers/Object.hpp
)
target_include_directories(Object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



add_library(
    stb_image
    sources/stb_image.cpp
    ../libs/stb/stb_image.h
)

set(STB_INCLUDE "../libs/stb")
target_include_directories(stb_image PUBLIC ${STB_INCLUDE})


add_subdirectory(sources)


