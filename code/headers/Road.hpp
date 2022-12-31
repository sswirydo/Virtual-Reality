#ifndef ROAD_HPP
#define ROAD_HPP

#include "Object.hpp"
#include "InstancedObject.hpp"
#include "Car.hpp"
#include "Camera.hpp"
#include "StreetLamp.hpp"

class Object;
class Car;

class Road : public Object
{
public:
    Road(Model* model, Shader* shader, Physics* physics, LightSource* light);
    ~Road();

    void addCarInfo(Model* model, Shader* shader, LightSource* light);

    void move(int nbOfRoads, int pos = -1);

    void addCars(std::vector<Car*> cars);
    void linkObject(Object* obj);
    std::vector<Object*> getLinkedObjects();
    std::vector<Car*> getCars();

    void addNewCars(glm::vec3 currentRoadTranslate);
    void checkCarsState();
    void removeCar(Car* car);

protected:

    void generateLamps();
    void generateTrees();
    void moveLinkedObjects(glm::vec3 vector);
    //void moveLinkedCars(glm::vec3 vector);

    std::vector<Object*> linkedObjects;
    std::vector<Car*> cars;



private:
    btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape);
    std::vector<btRigidBody*> otherBodies;
    void translateBarriers(glm::vec3 vector);

    Model* carModel = nullptr;
    Shader* carShader = nullptr;
    LightSource* carLight = nullptr;
};

#endif //! ROAD