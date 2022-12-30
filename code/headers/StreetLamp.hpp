#ifndef STREETLAMP_HPP
#define STREETLAMP_HPP
#include "Object.hpp"
class StreetLamp : public Object
{
private:
    bool isAtRight;
    LightSource my_light;
public:
    StreetLamp(Model& model, Shader& shader, Physics* physics, LightSource* light,bool isAtRight, int position);
    StreetLamp();
    void renderDuplicate(int n,Camera *camera);
    void drawLampLight(Camera *camera);
    void Draw();
    ~StreetLamp();
};



#endif