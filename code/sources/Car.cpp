#include "../headers/Car.hpp"

btCollisionShape* Car::carCollisionShape = nullptr;
Model* Car::carModel = nullptr;

Car::Car(Model* model, Shader* shader, Physics* physics, LightSource* light) : Object(model, shader, physics, light)
{
    // TODO: below is provisory (testing)
    if (Car::carCollisionShape == nullptr) {
        Car::carCollisionShape = new btBoxShape(btVector3((btScalar)1, (btScalar)0.85, (btScalar)1.75));
    }
    this->collisionShape = Car::carCollisionShape;
    btDefaultMotionState* carMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btScalar carMass = 1000;
    btVector3 carInertia(0, 0, 0);
    this->collisionShape->calculateLocalInertia(carMass, carInertia);
    btRigidBody::btRigidBodyConstructionInfo carRigidBodyCI(carMass, carMotionState, this->collisionShape, carInertia);
    this->rigidBody = new btRigidBody(carRigidBodyCI);

    //this->rigidBody->setFriction(0.9);
    this->rigidBody->setAngularFactor(btVector3(1, 0, 1)); // disables Y-axis rotation

    physics->addBody(rigidBody, CAR);

    // small initial motions
    btVector3 velocity = btVector3(0, 0, -10);
    this->getRigidBody()->setLinearVelocity(velocity);
    
}

Car::~Car() {
    this->collisionShape = nullptr;
}

void Car::move(float deltaTime) // TODO redo
{   
    //Update the car's position based on the velocity
    btTransform transform = this->getRigidBody()->getWorldTransform();
    btVector3 position = transform.getOrigin();
    position += btVector3(0, 0, -15) * deltaTime;
    transform.setOrigin(position);
    this->getRigidBody()->setWorldTransform(transform);
    this->getRigidBody()->activate(true);
}


glm::mat4 Car::getModelMatrix() 
{
    this->updateModelFromPhysics();
    this->translateModel(glm::vec3(0, -0.85, -0.05));
    this->rotateModel(180.0f, glm::vec3(0, 1, 0));
    return Object::getModelMatrix();
}

