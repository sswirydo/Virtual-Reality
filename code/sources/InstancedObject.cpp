#include "../headers/InstancedObject.hpp"

InstancedObject::InstancedObject(Model* model, Shader* shader, Physics* physics, LightSource* light, std::vector<glm::vec3> translations)
	: Object(model, shader, physics, light)
{
	this->translations = translations;
}

void InstancedObject::Draw()
{
	this->model->InstancedDraw(this->shader, this->translations);

}

