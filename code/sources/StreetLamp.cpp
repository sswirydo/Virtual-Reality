#include "../headers/StreetLamp.hpp"

StreetLamp::StreetLamp(Model& model, Shader& shader, Physics* physics, LightSource* sun, bool isAtRight, int position) : Object(model, shader, physics, sun) 
{
    this->isAtRight = isAtRight;
    this->my_light = LightSource(glm::vec3(0.0f),glm::vec4(232.0/255.0,144.0/255.0,240.0/5.0,1.0f));
    if(this->isAtRight){
        this->setModelMatrix(glm::scale(this->getModelMatrix(),glm::vec3(6.0f)));
        this->translateModel(glm::vec3(1.5f,0.0f,0.0f));
        this->rotateModel(180.0f,glm::vec3(0,1,0));
        this->translateModel(glm::vec3(0,0.0f,position));
    }
    else
    {
        this->translateModel(glm::vec3(-1.5f,0.0f,0.0f) + glm::vec3(0,0.0f,position));
    }
    glm::vec3 myLightPosition; 
    if(this->isAtRight){
        myLightPosition = glm::vec3(this->getModelMatrix()[3]) + glm::vec3(-1.8,5.3,0);
    }
    else{
        myLightPosition = glm::vec3(this->getModelMatrix()[3]) + glm::vec3(1.8,5.3,0);
    }
    this->my_light.setPosition(myLightPosition);
}

void StreetLamp::drawLampLight(Camera *camera){
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