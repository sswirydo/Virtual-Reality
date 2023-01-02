#include "../headers/Sun.hpp"

void Sun::rotate(glm::vec3 carPos){
    // implementation of day and night: we set a rotation radius of 250
    float framePerDegree = 10;
    double degree = (((this->frameNumber++)/framePerDegree) + 180);
    double angle = glm::radians(degree);
    this->position = glm::vec3(-250.0f,250.0f,-250.0f) * glm::vec3(glm::cos(angle),glm::sin(angle),glm::cos(angle)) + carPos;
    if(this->position.y >=0 )
        this->color = glm::vec4(glm::vec3(glm::abs(glm::sin(angle))),0.3f);
    else
        this->color = glm::vec4(0.0f,0.0f,0.0f,0.3f);
}

Sun::Sun(glm::vec3 lightPosition, glm::vec4 lightColor) : LightSource(lightPosition,lightColor)
{
    this->setAmbient(1.0);
    this->setDiffuse(1.0);
    this->setSpecular(1.0);
}

void Sun::setDirection(glm::vec3 direction)
{
    this->direction = direction;
}

glm::vec3 Sun::getDirection(){
    return this->direction;
}

bool Sun::isNight()
{
    return this->position.y <= 85;
}
Sun::~Sun()
{
}
