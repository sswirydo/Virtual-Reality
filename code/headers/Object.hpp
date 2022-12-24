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

// settings
const unsigned int SCR_WIDTH = 800*1.5; // 800x600 ? are you executing this on your phone or what ? :p
const unsigned int SCR_HEIGHT = 600*1.5;

class Object
{
protected:
    Model model;
    Shader shader;
    Camera *camera;
    glm::mat4 M;
    glm::mat4 V;
    glm::mat4 P;
public:
    Object(Model model,Shader &shader,Camera* camera);
    Object();
    void render();
    void setModel(Model model);
    void setShader(Shader shader);
    void setM(glm::mat4 model);
    void setV(glm::mat4 view);
    void setP(glm::mat4 projection);
    glm::mat4 getM();
    glm::mat4 getV();
    glm::mat4 getP();
    ~Object();
};


#endif