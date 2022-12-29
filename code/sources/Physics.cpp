#include "../headers/Physics.hpp"

btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape);

Physics::Physics() 
{
    // Provides default collision detection configuration and is responsible for creating and configuring the btCollisionDispatcher.
    this->collisionConfiguration = new btDefaultCollisionConfiguration();
    // Is responsible for dispatching collision detection requests to the appropriate collision detection algorithms.
    this->dispatcher = new btCollisionDispatcher(collisionConfiguration);
    // Broadphase collision detection algorithm that uses dynamic bounding volume trees to quicklyand efficiently determine which pairs of objects may potentially be colliding.
    this->broadphase = new btDbvtBroadphase();
    // Applies impulses to objects in order to resolve constraints (e.g., joint limits, contact constraints)
    this->solver = new btSequentialImpulseConstraintSolver;
    // Create a physics world that simulates a dynamics system using discrete time steps
    this->world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    this->world->setGravity(btVector3(0, -9.81, 0));
}

void Physics::addBody(btRigidBody* body, int gameObjectId)
{
    this->getWorld()->addRigidBody(body);
    BulletObject* bo = new BulletObject(body, gameObjectId);
    this->bulletObjects.push_back(bo);
    body->setUserPointer(bo);
}

btDiscreteDynamicsWorld* Physics::getWorld() 
{
    return this->world;
}

void Physics::addShape(btCollisionShape *collisionShape)
{
    this->collisionShapes.push_back(collisionShape);
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
