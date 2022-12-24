#include "../headers/Car.hpp"

Car::Car(Model model,Shader &shader, Camera *camera) : Object(model,shader, camera)
{
}

void Car::render(LightSource &light)
{
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 8000.0f);
    glm::mat4 view = camera->GetViewMatrix();
    this->setV(view);
    this->setP(projection);
    this->shader.use();
    this->shader.setVec3("lightPos",light.getPosition());
    this->shader.setVec4("lightColor",light.getColor());
    this->shader.setVec3("viewPos", this->camera->Position); 
    this->shader.setMat4("projection", this->P);
    this->shader.setMat4("view", this->V);
    this->shader.setMat4("model", this->M);
    this->model.Draw(this->shader);
}
Car::~Car()
{
}
