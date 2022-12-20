#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include "Texture.hpp"
#include "camera.hpp"
#include <string>
#include <iostream>
#include <vector>

class Object
{
protected:
    std::vector<float> vertices;
    std::vector<int> indices;
    GLuint VBO, VAO;
    Shader shader;
    Texture texture;

public:
    Object(std::string vertexShader, std::string fragmentShader);
    Object();
    Shader getShader();
    std::vector<float> getVertices();
    std::vector<int> getIndices();
    Texture getTexture();
    void setShader(Shader shader);
    void setIndices(std::vector<int> indices);
    void configure_VAO_VBO();
    void render();
    void setVertices(std::vector<float>vertices);
    void setTexture(Texture texture);
    void setTexture(std::string path);
    void terminate();
    void set_vertex_attributes_pointers(int layoutLocation, int vectSize, int stride, int offset);
    ~Object();
};


#endif