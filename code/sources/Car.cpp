#include "../headers/Car.hpp"

Car::Car(Model model, Shader &shader, Camera * camera, Physics* physics) : Object(model, shader, camera, physics) {}

void Car::render(LightSource &light)
{
    glm::mat4 projection = camera->GetProjectionMatrix();
    glm::mat4 view = camera->GetViewMatrix();

    this->shader.use();
    this->shader.setVec3("lightPos", light.getPosition());
    this->shader.setVec4("lightColor", light.getColor());
    this->shader.setVec3("viewPos", this->camera->Position); 
    this->shader.setMat4("projection", projection);
    this->shader.setMat4("view", view);
    this->shader.setMat4("model", this->modelMatrix);
    this->model.Draw(this->shader);
}
