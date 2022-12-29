#include "../headers/Physics.hpp"

btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape);

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

Physics::~Physics()
{
    for (int j = 0; j < collisionShapes.size(); j++)
    {
        btCollisionShape* shape = collisionShapes[j];
        collisionShapes[j] = 0;
        delete shape;
    }

    delete this->collisionConfiguration;
    delete this->dispatcher;
    delete this->solver;
    delete this->broadphase;
    delete this->world;
}
