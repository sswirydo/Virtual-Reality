#ifndef CAR_HPP
#define CAR_HPP
#include "Object.hpp"
#include "LightSource.hpp"


class Car : public Object
{
public:
    Car(Model model,Shader &shader, Camera * camera, Physics* physics);
    void move();
    void render(LightSource &light);
    btRigidBody* getCarBody();
    btCollisionShape* getCarShape();
    void renderShapeBox(Shader &shader);
private:
    btRigidBody* carBody;
    btCollisionShape* carShape;
};

#endif