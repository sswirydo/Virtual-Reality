#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <btBulletDynamicsCommon.h>

class Physics {

public:
	Physics();
	btDiscreteDynamicsWorld* getWorld();
	void addShape(btCollisionShape* collisionShape);
	~Physics();
protected:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* world;
private:
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

#endif // !PHYSICS_HPP
