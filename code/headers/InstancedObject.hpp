#ifndef INSTANCED_OBJECT_HPP
#define INSTANCED_OBJECT_HPP

#include "Object.hpp"

class Object;

class InstancedObject : public Object
{
public:
    InstancedObject();
    InstancedObject(Model* model, Shader* shader, Physics* physics, LightSource* light, std::vector<glm::vec3> translations);
    //~Object();

    void Draw();

protected:
    std::vector<glm::vec3> translations;

};


#endif //! INSTANCED_OBJECT_HPP