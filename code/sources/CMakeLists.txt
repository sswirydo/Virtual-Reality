add_executable(app.out main.cpp)

target_link_libraries(app.out PUBLIC LightSource Car Mesh Model Game Object Shader Texture stb_image OpenGL::GL glfw glad glm assimp)