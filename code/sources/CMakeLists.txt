add_executable(app.out main.cpp)

target_link_libraries(app.out PUBLIC Vertices Container Game Object Shader Texture stb_image OpenGL::GL glfw glad glm assimp)