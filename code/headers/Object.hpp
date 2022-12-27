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
#include "Camera.hpp"

#include <string>
#include <iostream>
#include <vector>

class Camera;

class Object
{
public:
    Object();
    Object(Model model, Shader &shader, Camera * Camera);
    void render();
    void setModel(Model model);
    void setShader(Shader shader);
    void setModelMatrix(glm::mat4 model);
    glm::mat4 getModelMatrix();
protected:
    Model model;
    Shader shader;
    Camera * camera;
    glm::mat4 modelMatrix;
};


#endif