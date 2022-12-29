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


    //keep track of the shapes, we release memory at exit.
    //make sure to re-use collision shapes among rigid bodies whenever possible!
    {
        //Creates the ground shape
        btCollisionShape* groundShape = new btBoxShape(btVector3(100, 1, 110));
        //Stores on an array for reusing
        collisionShapes.push_back(groundShape);
        //Creates the ground rigidbody
        btRigidBody* groundRigidBody = createGroundRigidBodyFromShape(groundShape);
        //Adds it to the world
        this->world->addRigidBody(groundRigidBody);
    }

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

btRigidBody* createGroundRigidBodyFromShape(btCollisionShape* groundShape)
{
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, -1, 0));

    {
        //The ground is not dynamic, we set its mass to 0
        btScalar mass(0);

        //No need for calculating the inertia on a static object
        btVector3 localInertia(0, 0, 0);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, groundMotionState, groundShape, localInertia);

        return new btRigidBody(rbInfo);
    }
}