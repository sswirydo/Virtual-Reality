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
        btCollisionShape* groundShape = new btBoxShape(btVector3(107, 1, 100));
        btCollisionShape* borneShape_l = new btBoxShape(btVector3(0.5, 1, 200));
        btCollisionShape* borneShape_r = new btBoxShape(btVector3(0.5, 1, 200));

        btDefaultMotionState* borneShape_l_ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-7, 0, 0)));
        btDefaultMotionState* borneShape_r_ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(7, 0, 0)));
        
        btRigidBody::btRigidBodyConstructionInfo borne_l_RigidBodyCI(0, borneShape_l_ms, borneShape_l);
        btRigidBody::btRigidBodyConstructionInfo borne_r_RigidBodyCI(0, borneShape_r_ms, borneShape_r);
        btRigidBody *rigidBody_l = new btRigidBody(borne_l_RigidBodyCI);
        btRigidBody *rigidBody_r = new btRigidBody(borne_r_RigidBodyCI);
        //Stores on an array for reusing
        collisionShapes.push_back(groundShape);
        collisionShapes.push_back(borneShape_l);
        collisionShapes.push_back(borneShape_r);
        //Creates the ground rigidbody
        btRigidBody* groundRigidBody = createGroundRigidBodyFromShape(groundShape);
        //Adds it to the world
        this->world->addRigidBody(groundRigidBody);
        this->world->addRigidBody(rigidBody_l);
        this->world->addRigidBody(rigidBody_r);
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
