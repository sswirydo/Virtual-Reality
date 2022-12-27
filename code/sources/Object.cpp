#include "../headers/Object.hpp"

Object::Object() {}

Object::Object(Model &model,Shader &shader, Camera* camera, Physics* physics)
{
    this->physics = physics;
    this->camera = camera;
    this->setModel(model);
    this->setShader(shader);
    this->modelMatrix = glm::mat4(1.0f);
}

void Object::setModel(Model &model){
    this->model = model;
}

void Object::setShader(Shader &shader){
    this->shader = shader;
}

void Object::setModelMatrix(glm::mat4 model)
{   
    this->modelMatrix = model;
}

glm::mat4 Object::getModelMatrix()
{
    return this->modelMatrix;
}

void Object::render()
{
    glm::mat4 projection = this->camera->GetProjectionMatrix();
    glm::mat4 view = this->camera->GetViewMatrix();
    this->shader.use();
    this->shader.setMat4("projection", projection);
    this->shader.setMat4("view", view);
    this->shader.setMat4("model", this->modelMatrix);
    this->model.Draw(this->shader);
}
