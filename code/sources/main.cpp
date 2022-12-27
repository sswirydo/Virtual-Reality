#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <stdio.h>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../headers/Game.hpp"
#include "../headers/Camera.hpp"
#include "../headers/Object.hpp"
#include "../headers/Car.hpp"
#include "../headers/LightSource.hpp"
#include "../headers/Physics.hpp"
#include "../headers/Debug.hpp"

#include "../headers/Skybox.hpp"


void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


// settings
const unsigned int SCR_WIDTH = 800*1.5; // 800x600 ? are you executing this on your phone or what ? :p
const unsigned int SCR_HEIGHT = 600*1.5;
float screenRatio = (float) SCR_WIDTH / (float) SCR_HEIGHT;

// camera
Camera camera(screenRatio, glm::vec3(0.0f, 3.0f, 7.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool processMouseInput = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
    Game game = Game("Racing Game", SCR_WIDTH, SCR_HEIGHT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    #ifndef NDEBUG
    glDebug();
    #endif // !NDEBUG
    
    // GLFW CALLBACKS
    glfwSetFramebufferSizeCallback(game.getWindow(), framebuffer_size_callback);
    glfwSetCursorPosCallback(game.getWindow(), mouse_callback);
    glfwSetScrollCallback(game.getWindow(), scroll_callback);
    glfwSetKeyCallback(game.getWindow(), key_callback);
    glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse

    Physics* physics = new Physics();

    Skybox skybox = Skybox(&camera);
    
    LightSource light(glm::vec3(-100.0f, 100.0f, -100.0f),glm::vec3(1.0f, 1.0f, 1.0f));

    Shader lightShader = Shader("code/shaders/lightShader.vert", "code/shaders/lightShader.frag");
    
    Shader carShader= Shader("code/shaders/car.vert","code/shaders/car.frag");
    Model carModel = Model("assets/meshes/free-car/free_car_001.obj");
    Car car = Car(carModel, carShader, &camera, physics);

    //double prev = 0;
    //int deltaFrame = 0;
    ////fps function
    //auto fps = [&](double now) {
    //    double deltaTime = now - prev;
    //    deltaFrame++;
    //    if (deltaTime > 0.5) {
    //        prev = now;
    //        const double fpsCount = (double)deltaFrame / deltaTime;
    //        deltaFrame = 0;
    //        std::cout << "\r FPS: " << fpsCount;
    //    }
    //};

    while (!glfwWindowShouldClose(game.getWindow()))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(game.getWindow());

        car.move();
        
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);
        car.renderShapeBox(lightShader);

        car.setModelMatrix(glm::translate(car.getModelMatrix(), glm::vec3(0.0f, -1.0f, 0.0f))); // TODO: TEMPORARY
        car.setModelMatrix(glm::rotate(car.getModelMatrix(), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))); // TODO: TEMPORARY

        car.render(light);
        light.show(&camera);

        

        

        skybox.render();
        glDepthFunc(GL_LESS);

        //fps(glfwGetTime());
        glfwSwapBuffers(game.getWindow());
        glfwPollEvents();

        //physics->getWorld()->stepSimulation(deltaTime); // <-- enable this for physics simulation
    }
    game.terminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// because processInput() is called every frame and we want the key to be processed only once
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        if (processMouseInput) 
        {
            processMouseInput = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            std::cout << "\nMOUSE DETACHED\n";
        }
        else 
        {
            processMouseInput = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            std::cout << "\nMOUSE ATTACHED\n";
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (processMouseInput)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


