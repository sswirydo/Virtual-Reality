#ifndef CONTAINER_HPP
#define CONTAINER_HPP
#include "Object.hpp"
#include "Vertices.hpp"

class Container : public Object
{
private:
    /* data */
public:
    Container(std::string vertexShader, std::string fragmentShader);
    void render();
    ~Container();
};

#endif