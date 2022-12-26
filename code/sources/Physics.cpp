#include "../headers/Physics.hpp"

Physics::Physics() 
{
    // Create a collision configuration
    this->collisionConfiguration = new btDefaultCollisionConfiguration();
    // Create a collision dispatcher
    this->dispatcher = new btCollisionDispatcher(collisionConfiguration);
    // Create a broadphase interface
    this->broadphase = new btDbvtBroadphase();
    // Create a constraint solver
    this->solver = new btSequentialImpulseConstraintSolver;
    // Create a physics world
    this->world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    // Set the gravity of the world
    this->world->setGravity(btVector3(0, -9.81, 0));
}

btDiscreteDynamicsWorld* Physics::getWorld() 
{
    return this->world;
}
