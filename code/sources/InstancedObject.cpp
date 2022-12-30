#include "../headers/InstancedObject.hpp"

InstancedObject::InstancedObject(Model* model, Shader* shader, Physics* physics, LightSource* light) : Object(model, shader, physics, light)
{

}

void InstancedObject::Draw()
{
	/* ... */
	Object::Draw(); // temp
}

void InstancedObject::render(Camera* camera) 
{
	/* ... */
	Object::render(camera); // temp
}