#ifndef CAR_HPP
#define CAR_HPP

#include "Object.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"

class Object;

class Car : public Object
{
public:
    Car(Model &model,Shader &shader, Camera * camera, Physics* physics);
    void move(float deltaTime, glm::vec4 direction);
    void render(LightSource &light);
    void Draw();

    std::vector<Mesh> getWheelsMesh();
    std::vector<Mesh> getWindowsMesh();
    std::vector<Mesh> getCarosserieMesh();

    void renderShapeBox(Shader &shader);

private:

};

#endif