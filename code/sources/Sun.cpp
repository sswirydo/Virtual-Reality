#include "../headers/Sun.hpp"

void Sun::rotate(glm::vec3 carPos){
    // implementation of day and night: we set a rotation radius of 250
    float framePerDegree = 10;
    double degree = (((this->frameNumber++)/framePerDegree) + 10);
    double angle = glm::radians(degree);
    this->position = glm::vec3(-250.0f,250.0f,-250.0f) * glm::vec3(glm::cos(angle),glm::sin(angle),glm::cos(angle)) + carPos;
    if(this->position.y >=0 )
        this->color = glm::vec4(glm::vec3(glm::abs(glm::sin(angle))),0.3f);
    else
        this->color = glm::vec4(0.0f,0.0f,0.0f,0.3f);
}

Sun::Sun(glm::vec3 lightPosition, glm::vec4 lightColor) : LightSource(lightPosition,lightColor)
{
    this->setAmbient(0.1);
    this->setDiffuse(0.8);
    this->setSpecular(0.5);
}

void Sun::setDirection(glm::vec3 direction)
{
    this->direction = direction;
}

glm::vec3 Sun::getDirection(){
    return this->direction;
}

Sun::~Sun()
{
}