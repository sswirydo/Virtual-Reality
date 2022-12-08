#include "../headers/Game.hpp"

Game::Game(){};

void Game::initOpenGL(std::string win_title, unsigned int width,unsigned int height)
{

    this->win_width = width;
    this->win_height = height;
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    this->window = glfwCreateWindow(width, height,win_title.c_str(), NULL, NULL);
    if (this->window == NULL)
    {
        std::runtime_error error("Failed to create GLFW window"); 
        throw error;
    }
    glfwMakeContextCurrent(this->window);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::runtime_error error("Failed to initialize GLAD"); 
        throw error;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int Game::getWidth(){
    return this->win_width;
}

unsigned int Game::getHeight(){
    return this->win_height;
}

GLFWwindow* Game::getWindow(){
    return this->window;
}

void Game::terminate(){
    glfwTerminate();
}

Game::~Game()
{
}