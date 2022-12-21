#include "../headers/Object.hpp"

Object::Object(){}

Object::Object(Model model,Shader shader)
{
    this->setModel(model);
    this->setShader(shader);
}

void Object::setModel(Model model){
    this->model = model;
}

void Object::setShader(Shader shader){
    this->shader = shader;
}

void Object::render(){
    this->model.Draw(this->shader);
}

Object::~Object()
{
   
}