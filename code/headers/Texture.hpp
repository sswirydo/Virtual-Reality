#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>

class Texture
{
private:
    GLuint id;
public:
    GLuint getId();
    Texture(std::string path);
    Texture();
    ~Texture();
};

#endif