#ifndef CAR_HPP
#define CAR_HPP

#include "Object.hpp"
#include "Camera.hpp"

class Object;

class Car : public Object
{
public:
    Car(Model &model,Shader &shader, Physics* physics,LightSource *light);
    void move(float deltaTime, glm::vec4 direction);
    // void render(Camera* camera);
    void Draw();
    std::vector<Mesh> getWheelsMesh();
    std::vector<Mesh> getWindowsMesh();
    std::vector<Mesh> getCarosserieMesh();

    void renderShapeBox(Camera* camera, Shader &shader);

private:

};

#endif