#ifndef LIGHT_SOURCE_HPP
#define LIGHT_SOURCE_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Camera.hpp"

const float vertices[] = {
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,

    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,

    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,

    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,

    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,

    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
};

class LightSource
{
protected:
    glm::vec3 position;
    glm::vec4 color;
    GLuint lightCubeVAO, VBO;
    Shader lightShader;
    glm::vec3 ambient = glm::vec3(1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f);
    glm::vec3 specular = glm::vec3(1.0f);

public:
    LightSource(glm::vec3 lightPosition, glm::vec4 lightColor);
    LightSource();
    glm::vec4 getColor();
    glm::vec3 getPosition();
    void setPosition(glm::vec3 newPosition);
    void setAmbient(float ambient);
    void setDiffuse(float diffuse);
    void setSpecular(float specular);
    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();
    void setColor(glm::vec4 newColor);
    void show(Camera *camera);
    ~LightSource();
};

#endif