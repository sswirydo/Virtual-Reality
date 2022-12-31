#ifndef SUN_HPP
#define SUN_HPP
#include "LightSource.hpp"
class Sun : public LightSource
{
private:
    glm::vec3 direction = glm::vec3(0,-1,0);
    unsigned int frameNumber = 0;
public:
    void rotate(glm::vec3 carPos);
    glm::vec3 getDirection();
    bool isNight();
    void setDirection(glm::vec3 newDirection);
    Sun(glm::vec3 lightPosition = glm::vec3(-250.0f, 250.0f, -250.0f), glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
    ~Sun();
};

#endif