#ifndef ROAD_HPP
#define ROAD_HPP

#include "Object.hpp"
#include "Camera.hpp"

class Object;

class Road : public Object
{
public:
    Road(Model& model, Shader& shader, Physics* physics, LightSource* light);
    void move(int nbOfRoads, int pos = -1);

    void addCars(std::vector<Car*> cars);
    void linkObject(Object* obj);
    std::vector<Object*> getLinkedObjects();

protected:

    void moveLinkedObjects(glm::vec3 vector);
    void moveLinkedCars(glm::vec3 vector);

    std::vector<Object*> linkedObjects;
    std::vector<Car*> cars;



private:
    btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape);
    std::vector<btRigidBody*> otherBodies;
    void translateBarriers(glm::vec3 vector);

};

#endif //! ROAD