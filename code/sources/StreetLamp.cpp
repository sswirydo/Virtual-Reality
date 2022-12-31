#include "../headers/StreetLamp.hpp"

constexpr int LAMP_SPACING = 40;
StreetLamp::StreetLamp(Model* model, Shader* shader, Physics* physics, LightSource* sun, bool isAtRight, int position) : Object(model, shader, physics, sun) 
{
    //this->setModelMatrix(glm::scale(this->getModelMatrix(),glm::vec3(6.0f)));
    this->isAtRight = isAtRight;
    glm::vec3 myLightPosition; 
    this->my_light = LightSource(glm::vec3(0.0f),glm::vec4(0.9f,0.7f,0.1f,1.0f));
    if(this->isAtRight){
        this->translateModel(glm::vec3(9.f,0.0f,-position* LAMP_SPACING));
        myLightPosition = glm::vec3(this->getModelMatrix()[3]) + glm::vec3(-3.8,8.3,0);
    }
    else
    {
        this->translateModel(glm::vec3(-9.f,0.0f,-position* LAMP_SPACING));
        myLightPosition = glm::vec3(this->getModelMatrix()[3])+ glm::vec3(3.8,8.3,0);
    }
    this->my_light.setPosition(myLightPosition);
    
}

void StreetLamp::render(Camera* camera, std::vector<StreetLamp*> lamps)
{
    this->Object::render(camera,lamps);
    glm::vec3 myLightPosition; 
    if(this->isAtRight)
        myLightPosition = glm::vec3(this->getModelMatrix()[3]) + glm::vec3(-3.8,8.3,0);
    else
        myLightPosition = glm::vec3(this->getModelMatrix()[3])+ glm::vec3(3.8,8.3,0);
    this->my_light.setPosition(myLightPosition);
    this->my_light.show(camera);
}

StreetLamp::~StreetLamp()
{
}
StreetLamp::StreetLamp()
{
}

LightSource StreetLamp::getLightSource()
{
    return this->my_light;
}