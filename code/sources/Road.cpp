#include "../headers/Road.hpp"

Road::Road(Model& model, Shader& shader, Physics* physics, LightSource* light) : Object(model, shader, physics, light) 
{
    //Creates the ground shape
    btCollisionShape* groundShape = new btBoxShape(btVector3(107, 1, 100));
    //Stores on an array for reusing
    //physics->collisionShapes.push_back(groundShape);
    //Creates the ground rigidbody
    btRigidBody* groundRigidBody = createGroundRigidBodyFromShape(groundShape);
    //Adds it to the world
    physics->getWorld()->addRigidBody(groundRigidBody);
}

void Road::move(int pos) 
{

}

btRigidBody* Road::createGroundRigidBodyFromShape(btCollisionShape* groundShape)
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