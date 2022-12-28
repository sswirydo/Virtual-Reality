#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Object.hpp"
#include "Car.hpp"
#include "Camera.hpp"
#include "LightSource.hpp"

class Object;

class Player : public Car
{

public:
    Player (Model& model, Shader& shader, Physics* physics, LightSource &light);
private:

};

#endif