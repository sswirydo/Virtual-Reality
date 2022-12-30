#include "../headers/StreetLamp.hpp"

constexpr int LAMP_SPACING = 40;
StreetLamp::StreetLamp(Model& model, Shader& shader, Physics* physics, LightSource* sun, bool isAtRight, int position) : Object(model, shader, physics, sun) 
{
    //this->setModelMatrix(glm::scale(this->getModelMatrix(),glm::vec3(6.0f)));
    this->isAtRight = isAtRight;
    glm::vec3 myLightPosition; 
    this->my_light = LightSource(glm::vec3(0.0f),glm::vec4(1.0f,1.0f,1.0f,1.0f));
    if(this->isAtRight){
        this->translateModel(glm::vec3(9.f,0.0f,position* LAMP_SPACING));
        myLightPosition = glm::vec3(this->getModelMatrix()[3]) + glm::vec3(-3.8,8.7,0);
    }
    else
    {
        this->translateModel(glm::vec3(-9.f,0.0f,position* LAMP_SPACING));
        myLightPosition = glm::vec3(this->getModelMatrix()[3])+ glm::vec3(3.8,8.7,0);
    }
    this->my_light.setPosition(myLightPosition);
    
}

void StreetLamp::render(Camera *camera)
{
    this->Object::render(camera);
    this->my_light.show(camera);
}

void StreetLamp::renderDuplicate(int n, Camera *camera)
{
    for (size_t i = 0; i < n; i++)
    {
        glm::mat4 model(1.0f);
        model = glm::scale(model,glm::vec3(6.0f));
        if(this->isAtRight){
            model = glm::translate(model,glm::vec3(1.5f,0.0f,0.0f));
            model = glm::rotate(model,glm::radians(180.0f),glm::vec3(0,1,0));
            model = glm::translate(model,glm::vec3(0,0,i*(50/n)));

        }
        else{
            // model = glm::rotate(model,glm::radians(-180.0f),glm::vec3(0,1,0));
            model = glm::translate(model,glm::vec3(-1.5f,0.0f,0.0f));
            model = glm::rotate(model,glm::radians(-180.0f),glm::vec3(0,1,0));
            model = glm::translate(model,glm::vec3(0,0,i*(50/n)));
            model = glm::rotate(model,glm::radians(-180.0f),glm::vec3(0,1,0));

        }
        this->setModelMatrix(model);
        
    }
    
}


StreetLamp::~StreetLamp()
{
}
StreetLamp::StreetLamp()
{
}