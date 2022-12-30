#ifndef INSTANCED_OBJECT_HPP
#define INSTANCED_OBJECT_HPP

#include "Object.hpp"

class Object;

class InstancedObject : public Object
{
public:
    InstancedObject();
    InstancedObject(Model* model, Shader* shader, Physics* physics, LightSource* light);
    //~Object();

    void render(Camera* camera);
    void Draw();
};


#endif //! INSTANCED_OBJECT_HPP