#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include <string>

class Window
{
public:
    Window(std::string win_title, unsigned int width, unsigned int height);
    GLFWwindow* getWindow();
    unsigned int getWidth();
    unsigned int getHeight();
    void terminate();
protected:
    void setCallbacks();
private:
    unsigned int win_width;
    unsigned int win_height;
    GLFWwindow* window;
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif