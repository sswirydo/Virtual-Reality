#ifndef CAR_HPP
#define CAR_HPP

#include "Object.hpp"
#include "Camera.hpp"

class Object;

class Car : public Object
{
public:
    Car(Model* model,Shader* shader, Physics* physics,LightSource* light);
    ~Car();
    void move(float deltaTime);
    void render(Camera* camera, std::vector<StreetLamp*> lamps);
    void Draw();
    std::vector<Mesh> getWheelsMesh();
    std::vector<Mesh> getWindowsMesh();
    std::vector<Mesh> getCarosserieMesh();

    glm::mat4 getModelMatrix();

    static void staticRender(std::vector<glm::mat4> modelMatrices, Camera* camera, std::vector<StreetLamp*> lamps);

private:
    static btCollisionShape* carCollisionShape;
    static Model* carModel;


};

#endif