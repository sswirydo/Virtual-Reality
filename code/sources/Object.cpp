#include "../headers/Object.hpp"

Object::Object(){}

Object::Object(Model model,Shader &shader, Camera *camera)
{
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 8000.0f);
    glm::mat4 view = camera->GetViewMatrix();
    this->camera = camera;
    this->setModel(model);
    this->setShader(shader);
    this->setV(view);
    this->setP(projection);
    this->M = glm::mat4(1.0f);
}

void Object::setModel(Model model){
    this->model = model;
}

void Object::setShader(Shader shader){
    this->shader = shader;
}

void Object::setM(glm::mat4 model)
{   
    this->M = model;
}
void Object::setV(glm::mat4 view)
{
    this->V = view;
}
void Object::setP(glm::mat4 projection)
{
    this->P = projection;
}
glm::mat4 Object::getM()
{
    return this->M;
}
glm::mat4 Object::getV()
{
    return this->V;
}
glm::mat4 Object::getP()
{
    return this->P;
}
void Object::render()
{
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 8000.0f);
    glm::mat4 view = camera->GetViewMatrix();
    this->setV(view);
    this->setP(projection);
    this->shader.use();
    this->shader.setMat4("projection", this->P);
    this->shader.setMat4("view", this->V);
    this->shader.setMat4("model", this->M);
    this->model.Draw(this->shader);
}

Object::~Object()
{
   
}