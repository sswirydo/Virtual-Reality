#include "../headers/Window.hpp"

Window::Window(std::string win_title, unsigned int width, unsigned int height)
{
    try 
    {
        this->win_width = width;
        this->win_height = height;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        this->window = glfwCreateWindow(width, height, win_title.c_str(), NULL, NULL);
        if (this->window == NULL)
        {
            std::runtime_error error("Failed to create GLFW window");
            throw error;
        }
        //glfwSetWindowAspectRatio(window, 1, 1);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwMakeContextCurrent(this->window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::runtime_error error("Failed to initialize GLAD");
            throw error;
        }
        glEnable(GL_DEPTH_TEST);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int Window::getWidth()
{
    return this->win_width;
}

unsigned int Window::getHeight()
{
    return this->win_height;
}

GLFWwindow* Window::getWindow()
{
    return this->window;
}

void Window::terminate()
{
    glfwTerminate();
}
