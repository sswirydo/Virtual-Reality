#ifndef Game_HPP
#define Game_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include <string>

class Game
{
private:
// settings
    unsigned int win_width;
    unsigned int win_height;
    GLFWwindow* window;
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
public:
    GLFWwindow* getWindow();
    unsigned int getWidth();
    unsigned int getHeight();
    
    void terminate();
    Game();
    void initOpenGL(std::string win_title, unsigned int width, unsigned int height);
    ~Game();
};

#endif