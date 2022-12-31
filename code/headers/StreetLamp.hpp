#ifndef STREETLAMP_HPP
#define STREETLAMP_HPP
#include "Object.hpp"

class Object;
class StreetLamp : public Object
{
private:
    bool isAtRight;
    LightSource my_light;
public:
    StreetLamp(Model* model, Shader* shader, Physics* physics, LightSource* light, bool isAtRight, int position);
    StreetLamp();
    LightSource getLightSource();
    void render(Camera* camera, std::vector<StreetLamp*> lamps);
    ~StreetLamp();
};



#endif