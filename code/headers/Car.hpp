#ifndef CAR_HPP
#define CAR_HPP
#include "Object.hpp"
#include "LightSource.hpp"


class Car : public Object
{
private:
    /* data */
public:
    Car(Model model,Shader &shader, Camera *camera);
    void render(LightSource &light);
    ~Car();
};

#endif