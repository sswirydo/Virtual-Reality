#ifndef STREETLAMP_HPP
#define STREETLAMP_HPP
#include "Object.hpp"
class StreetLamp : public Object
{
private:
    bool isAtRight;
    LightSource my_light;
public:
    StreetLamp(Model& model, Shader& shader, Physics* physics, LightSource* light,bool isAtRight);
    StreetLamp();
    void renderDuplicate(int n,Camera *camera);

    ~StreetLamp();
};



#endif