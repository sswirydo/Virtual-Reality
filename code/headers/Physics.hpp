#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <btBulletDynamicsCommon.h>

#include <vector>

// All the types of objects with rigid bodies
enum GameObjects {
	BARRIER = -2,
	ROAD = -1,
	DEFAULT = 0,
	PLAYER = 1,
	CAR = 2,
};

struct BulletObject {
	int id;
	btRigidBody* body;
	bool hit;
	BulletObject(btRigidBody* b, int i) : body(b), id(i), hit(false) {};
};


class Physics {

public:
	Physics();
	btDiscreteDynamicsWorld* getWorld();
	void addShape(btCollisionShape* collisionShape);
	void addBody(btRigidBody* body, int gameObjectId); // important to call for collision detection (otherwise seg fault)
	~Physics();
protected:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broadphase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* world;
	std::vector<BulletObject*> bulletObjects;
private:
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};

#endif // !PHYSICS_HPP
