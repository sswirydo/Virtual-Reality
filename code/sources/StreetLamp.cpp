#include "../headers/StreetLamp.hpp"

StreetLamp::StreetLamp(Model& model, Shader& shader, Physics* physics, LightSource* light, bool isAtRight) : Object(model, shader, physics, light) 
{
    this->isAtRight = isAtRight;
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
        this->render(camera);
    }
    
}


StreetLamp::~StreetLamp()
{
}
StreetLamp::StreetLamp()
{
}