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

#include "../headers/Settings.hpp"

#include "../headers/Game.hpp"
#include "../headers/Camera.hpp"
#include "../headers/Object.hpp"
#include "../headers/Car.hpp"
#include "../headers/Road.hpp"
#include "../headers/Player.hpp"
#include "../headers/LightSource.hpp"
#include "../headers/Physics.hpp"
#include "../headers/Debug.hpp"
#include "../headers/Skybox.hpp"
#include "../headers/PlayerCamera.hpp"
#include "../headers/WorldCamera.hpp"
#include "../headers/StreetLamp.hpp"

#include "../headers/DebugDrawer.hpp"   


void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// game settings
bool processMouseInput = true;
bool pauseGame = false;
glm::vec4 movementDirection = glm::vec4(false,false,false,false);

// camera
Camera* camera = NULL;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
unsigned cameraNum = 1;
unsigned cameraChanged = false;

bool renderDebug = false;
bool renderModel = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


// Based on https://www.youtube.com/watch?v=YweNArzAHs4
bool contactAddedCallbackBullet(
    btManifoldPoint& cp,
    const btCollisionObjectWrapper* colObj0,
    int partId0,
    int index0,
    const btCollisionObjectWrapper* colObj1,
    int partId1,
    int index1)
{    
    BulletObject* bo0 = (BulletObject*)colObj0->getCollisionObject()->getUserPointer(); // PLAYER
    BulletObject* bo1 = (BulletObject*)colObj1->getCollisionObject()->getUserPointer(); // OTHER
    if (bo1->id == CAR) {
        bo0->hit = true; // PLAYER WAS HIT :(
    }
    return false;
}


int main()
{
    std::cout << ">>> PROGRAM START <<<" << std::endl;

    Game game = Game("Racing Game", SCR_WIDTH, SCR_HEIGHT);
    gContactAddedCallback = contactAddedCallbackBullet;

    #ifndef NDEBUG
    glDebug();
    #endif // !NDEBUG
    
    // GLFW CALLBACKS
    glfwSetFramebufferSizeCallback(game.getWindow(), framebuffer_size_callback);
    glfwSetCursorPosCallback(game.getWindow(), mouse_callback);
    glfwSetScrollCallback(game.getWindow(), scroll_callback);
    glfwSetKeyCallback(game.getWindow(), key_callback);
    glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse

    WorldCamera worldCamera(glm::vec3(0.0f, 3.0f, 7.0f));
    Physics* physics = new Physics();

    LightSource sun = LightSource();

    Shader lightShader = Shader("code/shaders/lightShader.vert", "code/shaders/lightShader.frag");
    
    Shader carShader= Shader("code/shaders/car.vert","code/shaders/car.frag");
    Model carModel = Model("assets/meshes/car/car.obj");
    Player* playerCar = new Player(carModel, carShader, physics,&sun);

    Shader roadShader= Shader("code/shaders/road.vert","code/shaders/road.frag");
    Model roadModel = Model("assets/meshes/road/road.obj");
    Road road = Road(roadModel, roadShader, physics, &sun);
    Road road2 = Road(roadModel, roadShader, physics, &sun);
    Road road3 = Road(roadModel, roadShader, physics,&sun);
    std::vector<Road> roads;
    roads.push_back(road);
    roads.push_back(road2);
    roads.push_back(road3);
    for (size_t t = 0; t < roads.size(); t++) {
        roads[t].addCarInfo(carModel, carShader, &sun);
    }


    // TODO: temp tree add test
    Shader treeShader = Shader("code/shaders/tree.vert", "code/shaders/tree.frag");
    Model treeModel = Model("assets/meshes/tree/tree.obj");
    for (int i = 0; i < 1000; i++) { 
        int minZ = 0;
        int maxZ = 99;
        int minX = 9;
        int maxX = 100;

        int rangeZ = maxZ - minZ + 1;
        int rangeX = maxX - minX + 1;
        int numZ = rand() % rangeZ + minZ;
        int numX = rand() % rangeX + minX;

        Object* tree = new Object(treeModel, treeShader, physics, &sun);

        if (i % 4 == 0) { numZ = -numZ; }
        if (i % 4 == 1) { numX = -numX; }
        if (i % 4 == 2) { numZ = -numZ; numX = -numX; }

        glm::vec3 vector = glm::vec3(numX, 0, numZ);
        tree->setModelMatrix(glm::translate(tree->getModelMatrix(), vector));

        roads[i % roads.size()].linkObject(tree);
    }

    for (size_t t = 0; t < roads.size(); t++) {
        roads[t].move(roads.size(), t);
    }

    

    PlayerCamera playerCamera = PlayerCamera(playerCar);

    



    Skybox skybox = Skybox(&sun);

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

    int roadDisplacement = 3;
    DebugDrawer debugDrawer = DebugDrawer();
    physics->getWorld()->setDebugDrawer(&debugDrawer);

    while (!glfwWindowShouldClose(game.getWindow()))
    {
        if (cameraChanged) 
        {
            worldCamera.position = playerCamera.position;
            worldCamera.pitch = playerCamera.pitch;
            worldCamera.yaw = playerCamera.yaw;
            cameraChanged = false;
        }
        if (cameraNum == 1) 
        {
            camera = &playerCamera;
        }
        else if (cameraNum == 2)
        {
            camera = &worldCamera;
        }

 
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(game.getWindow());

        if (!pauseGame && !playerCar->wasHit()) 
        {
            playerCar->move(deltaTime, movementDirection);
        }
        

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);
        skybox.render(camera);


        float distance = playerCar->getWorldCoordinates().z;
        if (distance < -200 * (roadDisplacement - 2))
        {
            for (size_t t = 0; t < roads.size(); t++) {
                if (roadDisplacement % roads.size() == t) {
                    roads[t].move(roads.size());
                }
            }
            roadDisplacement++;
        }

       
        if (renderDebug) 
        {
            debugDrawer.setCamera(camera);
            physics->getWorld()->debugDrawWorld();
        }
        if (renderModel)
        {
            glm::vec3 newLightPosition = glm::vec3(playerCar->getModelMatrix()[3]);
            if (!pauseGame) {
                sun.rotate(newLightPosition);
            }
            sun.show(camera);

            // rendering the roads and objects 1st for transparent windows
            for (size_t t = 0; t < roads.size(); t++) {
                roads[t].render(camera);
                std::vector<Object*> linkedObjects = roads[t].getLinkedObjects();
                for (size_t l = 0; l < linkedObjects.size(); l++) {
                    linkedObjects[l]->render(camera);
                }
            }

            // rendering the cars later
            for (size_t t = 0; t < roads.size(); t++) {
                std::vector<Car*> linkedCars = roads[t].getCars();
                for (size_t l = 0; l < linkedCars.size(); l++) {
                    if (!pauseGame) {
                        linkedCars[l]->move(deltaTime);
                    }
                    linkedCars[l]->render(camera);
                } 
            }

            playerCar->render(camera);
        }
        
        glDepthFunc(GL_LESS);





        //fps(glfwGetTime());
        glfwSwapBuffers(game.getWindow());
        glfwPollEvents();

        if (!pauseGame) 
        {
            physics->getWorld()->stepSimulation(deltaTime); // <-- enable this for physics simulation
        }

        
    }
    game.terminate();
    delete playerCar;
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->ProcessKeyboard(FAST, deltaTime);
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        camera->ProcessKeyboard(SLOW, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // forward
        movementDirection.x = true;
    else
        movementDirection.x = false;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // backward
        movementDirection.y = true;
    else
        movementDirection.y = false;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // left
        movementDirection.z = true;
    else
        movementDirection.z = false;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // right
        movementDirection.w = true;
    else
        movementDirection.w = false;
    
}

// because processInput() is called every frame and we want the key to be processed only once
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        if (processMouseInput) {processMouseInput = false; glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); std::cout << "\nMOUSE DETACHED\n";}
        else {processMouseInput = true; glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); std::cout << "\nMOUSE ATTACHED\n";}
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) 
    {
        if (pauseGame) {pauseGame = false; std::cout << ">> Game resumed" << std::endl;}
        else {pauseGame = true; std::cout << ">> Game paused" << std::endl;}
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        if (renderDebug) { renderDebug = false; std::cout << ">> DEBUG OFF" << std::endl; }
        else { renderDebug = true; std::cout << ">> DEBUG ON" << std::endl; }
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if (renderModel) { renderModel = false; std::cout << ">> MODEL OFF" << std::endl; }
        else { renderModel = true; std::cout << ">> MODEL ON" << std::endl; }
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        cameraNum = 1; cameraChanged = true;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        cameraNum = 2; cameraChanged = true;
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

        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}


