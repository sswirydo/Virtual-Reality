#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <iostream>
#include <vector>
class Object
{
private:
    std::vector<float> vertices;
    std::vector<int> indices;
public:
    Object();
    Object(std::vector<float>, std::vector<int>);
    std::vector<float> getVertices();
    void setVertices(std::vector<float>vertices);
    std::vector<int> getIndices();
    void setIndices(std::vector<int> indices);
    ~Object();
};


#endif