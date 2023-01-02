#ifndef CAR_HPP
#define CAR_HPP

#include "Object.hpp"
#include "Camera.hpp"

class Object;

class Car : public Object
{
public:
    Car(Model* model,Shader* shader, Physics* physics,LightSource* light);
    ~Car();
    void move(float deltaTime);

    glm::mat4 getModelMatrix();

private:
    static btCollisionShape* carCollisionShape;
    static Model* carModel;


};

#endif