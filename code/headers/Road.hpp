#ifndef ROAD_HPP
#define ROAD_HPP

#include "Object.hpp"
#include "Camera.hpp"

class Object;

class Road : public Object
{
public:
    Road(Model& model, Shader& shader, Physics* physics, LightSource* light);
    void move(int pos);

private:
    btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape);
    std::vector<btRigidBody*> otherBodies;
    void translateBarriers(int pos);

};

#endif //! ROAD