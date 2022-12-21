#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include "Model.hpp"
#include <string>
#include <iostream>
#include <vector>

class Object
{
protected:
    Model model;
    Shader shader;
public:
    Object(Model model,Shader shader);
    Object();
    void render();
    void setModel(Model model);
    void setShader(Shader shader);
    ~Object();
};


#endif