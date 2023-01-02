
add_library(
    stb_image STATIC
    sources/stb_image.cpp
    ../libs/stb/stb_image.h
)
set(STB_INCLUDE "../libs/stb")
target_include_directories(stb_image PUBLIC ${STB_INCLUDE})

add_library(
    stb_truetype STATIC
    sources/stb_image.cpp
    ../libs/stb/stb_truetype.h
)
target_include_directories(stb_truetype PUBLIC ${STB_INCLUDE})

add_subdirectory(sources)
