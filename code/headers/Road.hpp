#ifndef ROAD_HPP
#define ROAD_HPP

#include "Object.hpp"
#include "Car.hpp"
#include "Camera.hpp"
#include "StreetLamp.hpp"

class Object;
class Car;

class Road : public Object
{
public:
    Road(Model& model, Shader& shader, Physics* physics, LightSource* light);
    void move(int nbOfRoads, int pos = -1);

    void addCars(std::vector<Car*> cars);
    void linkObject(Object* obj);
    std::vector<Object*> getLinkedObjects();
    std::vector<Car*> getCars();

protected:

    void generateLamps();
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