
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../headers/Settings.hpp"

#include "../headers/Window.hpp"
#include "../headers/Camera.hpp"
#include "../headers/Object.hpp"
#include "../headers/InstancedObject.hpp"
#include "../headers/Car.hpp"
#include "../headers/CarRenderer.hpp"
#include "../headers/Road.hpp"
#include "../headers/Player.hpp"
#include "../headers/Sun.hpp"
#include "../headers/Physics.hpp"
#include "../headers/Skybox.hpp"
#include "../headers/PlayerCamera.hpp"
#include "../headers/WorldCamera.hpp"
#include "../headers/StreetLamp.hpp"
#include "../headers/Sound.hpp"
#include "../headers/Text.hpp"
#include "../headers/Highscores.hpp"

#include "../headers/Debug.hpp"
#include "../headers/DebugDrawer.hpp"   

enum Scene {
    MENU,
    PLAY,
    HELP,
};

    //////////////////
    // DECLARATIONS //
    //////////////////

int main();
void setCallbacks(GLFWwindow* window);
void initShaders();
void initModels();
void initTexts(Scene scene);
void init();
int play();

void processInput(GLFWwindow* window);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    //////////////////////////
    // VARIABLES AND OTHERS //
    //////////////////////////



// Principal arguments //
Window* window = nullptr;

// Bullets //
Physics* physics = nullptr;
DebugDrawer* debugDrawer = nullptr;

// Objects //
Player* playerCar = nullptr;
std::vector<Road*> roads;
std::vector<StreetLamp*> lamps;

// Models //
Model* carModel = nullptr;
Model* roadModel = nullptr;

// Cameras //
Camera* camera = nullptr;
WorldCamera* worldCamera = nullptr;
PlayerCamera* playerCamera = nullptr;

// Shaders //
Shader* lightShader = nullptr;
Shader* roadShader = nullptr;
Shader* carShader = nullptr;
Shader* carInstancedShader = nullptr;
Shader* fontShader = nullptr;

// Light //
Sun* sun = nullptr;
Skybox* skybox = nullptr;

// Render //
CarRenderer* carRenderer = nullptr;

// Texts //
Font* font = nullptr;
Text* score_text = nullptr;
Text* speed_text = nullptr;
Text* time_text = nullptr;
Text* highscores_title_text = nullptr;
std::vector<Text*> best_scores_textes;

// Variables //
int roadDisplacement = NULL;
int score = NULL;
std::string playerName = "PLAYER";


// Game settings //
bool processMouseInput = true;
bool pauseGame = false;
glm::vec4 movementDirection = glm::vec4(false, false, false, false);
bool enableFog = true;
bool enableSpeed = true;
bool enableText = true;

// Camera settings //
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
unsigned cameraNum = 1;
unsigned cameraChanged = false;
bool renderDebug = false;
bool renderModel = true;

// Window //
const int frameRate = 120;
const int frameDelay = (int)(1000 / frameRate);

// Time //
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
    // std::cout << "Collision of id " << bo0->id << " with id " << bo1->id << std::endl;
    if (bo1->id == CAR) {
        bo0->hit = true; // PLAYER WAS HIT :(
    }
    return false;
};



    //////////
    // MAIN //
    //////////

int main()
{
    std::cout << ">>> PROGRAM START <<<" << std::endl;
    window = new Window("Racing Game", SCR_WIDTH, SCR_HEIGHT);
    setCallbacks(window->getWindow());

    gContactAddedCallback = contactAddedCallbackBullet;
    debugDrawer = new DebugDrawer();

    ReadHighScores();

    initShaders();
    initModels();
    initTexts(PLAY);
    init();

    play();



    std::cout << ">>> PROGRAM END <<<" << std::endl;
#ifndef NDEBUG
    glDebug();
#endif // !NDEBUG
};



    ///////////
    // INITS //
    ///////////

void setCallbacks(GLFWwindow* window)
{
    // GLFW CALLBACKS
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse
}

void initShaders()
{
    lightShader = new Shader("code/shaders/lightShader.vert", "code/shaders/lightShader.frag");
    carShader = new Shader("code/shaders/car.vert", "code/shaders/car.frag");
    carInstancedShader = new Shader("code/shaders/instancedMatrixObject.vert", "code/shaders/car.frag");
    roadShader = new Shader("code/shaders/textureLessShader.vert", "code/shaders/textureLessShader.frag");
    fontShader = new Shader("code/shaders/font.vert", "code/shaders/font.frag");
}

void initModels()
{
    carModel = new Model("assets/meshes/car/car.obj");
    roadModel = new Model("assets/meshes/road/road.obj");
}

void initTexts(Scene scene)
{
    if (MENU)
    {

    }
    if (PLAY)
    {
        font = new Font("assets/fonts/BebasNeue-Regular.ttf");
        score_text = new Text(font, glm::vec2(10, 50));
        speed_text = new Text(font, glm::vec2(10, 50 * 2));
        time_text = new Text(font, glm::vec2(10, 50 * 3));
        highscores_title_text = new Text(font, glm::vec2(10, 400 - 50)); // TODO: modify text vec2 with screen resolution
        highscores_title_text->Update("HIGHSCORES");
        Text* highscores_top1_text = new Text(font, glm::vec2(10, 400)); // TODO: modify text vec2 with screen resolution
        Text* highscores_top2_text = new Text(font, glm::vec2(10, 400 + 50));
        Text* highscores_top3_text = new Text(font, glm::vec2(10, 400 + 50 * 2));
        Text* highscores_top4_text = new Text(font, glm::vec2(10, 400 + 50 * 3));
        Text* highscores_top5_text = new Text(font, glm::vec2(10, 400 + 50 * 4));
        best_scores_textes = std::vector<Text*>();
        best_scores_textes.push_back(highscores_top1_text);
        best_scores_textes.push_back(highscores_top2_text);
        best_scores_textes.push_back(highscores_top3_text);
        best_scores_textes.push_back(highscores_top4_text);
        best_scores_textes.push_back(highscores_top5_text);
    }
    if (HELP)
    {

    }
}

void init()
{
    physics = new Physics();
    physics->getWorld()->setDebugDrawer(debugDrawer);

    sun = new Sun();
    carRenderer = new CarRenderer(carModel, carInstancedShader, sun);
    worldCamera = new WorldCamera(glm::vec3(0.0f, 3.0f, 7.0f));
    skybox = new Skybox(sun);

    playerCar = new Player(carModel, carShader, physics, sun);
    playerCamera = new PlayerCamera(playerCar);

    Road* road = new Road(roadModel, roadShader, physics, sun);
    Road* road2 = new Road(roadModel, roadShader, physics, sun);
    Road* road3 = new Road(roadModel, roadShader, physics, sun);
    roads = std::vector<Road*>();
    roads.push_back(road);
    roads.push_back(road2);
    roads.push_back(road3);
    lamps = std::vector<StreetLamp*>();
    for (size_t t = 0; t < roads.size(); t++) {
        roads[t]->addCarInfo(carModel, carShader, sun);
        roads[t]->move((int)roads.size(), (int)t);
        for (size_t i = 0; i < roads[t]->getLamps().size(); i++)
            lamps.push_back(roads[t]->getLamps()[i]);
    }
    roadDisplacement = (int)roads.size();
    score = 0;
}



    //////////
    // PLAY //
    //////////


int play()
{
    auto score_start_time = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window->getWindow()))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //  Camera selection.
        if (cameraChanged)
        {
            worldCamera->position = playerCamera->position;
            worldCamera->pitch = playerCamera->pitch;
            worldCamera->yaw = playerCamera->yaw;
            cameraChanged = false;
        }
        if (cameraNum == 1) { camera = playerCamera; }
        else if (cameraNum == 2) { camera = worldCamera; }

        // Input handling.
        processInput(window->getWindow());
        if (!pauseGame && !playerCar->wasHit())
        {
            playerCar->move(deltaTime, movementDirection, roadDisplacement, enableSpeed);
        }

        // Moving road segments as player advances in game.
        float distance = playerCar->getWorldCoordinates().z;
        if (distance < -200 * (roadDisplacement - 2)) // todo check if it is still -2 for more than 3 roads
        {
            for (size_t t = 0; t < roads.size(); t++) {
                if (roadDisplacement % roads.size() == t) {
                    roads[t]->move((int)roads.size());
                }
            }
            roadDisplacement++;
        }

        // Advances physics simulation.
        if (!pauseGame) { physics->getWorld()->stepSimulation(deltaTime); }

        // Graphics cleaning.
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_LEQUAL);

        // Skybox rendering.
        skybox->render(camera, lamps, enableFog);
        //glDepthFunc(GL_LESS);

        // Moving the sun.
        glm::vec3 newLightPosition = glm::vec3(playerCar->getModelMatrix()[3]);
        if (!pauseGame) { sun->rotate(newLightPosition); }

        // Setting up timer for framerate cap.
        auto startTime = std::chrono::high_resolution_clock::now();


        // Rendering collision boxes from Bullet Physics.
        if (renderDebug)
        {
            debugDrawer->setCamera(camera);
            physics->getWorld()->debugDrawWorld();
        }

        // Getting all model matrices of cars for instancing
        std::vector<glm::mat4> carModelMatrices = std::vector<glm::mat4>();
        for (size_t t = 0; t < roads.size(); t++) {
            std::vector<Car*> linkedCars = roads[t]->getCars();
            for (size_t l = 0; l < linkedCars.size(); l++) {
                if (!pauseGame) {
                    linkedCars[l]->move(deltaTime);
                }
                carModelMatrices.push_back(linkedCars[l]->getModelMatrix());
            }
        }
        carModelMatrices.push_back(playerCar->getModelMatrix());


        // Rendering our models.
        if (renderModel)
        {
            sun->show(camera);
            // Rendering the roads and objects 1st for transparent windows.
            for (size_t t = 0; t < roads.size(); t++) {
                roads[t]->render(camera, lamps, enableFog);
                std::vector<Object*> linkedObjects = roads[t]->getLinkedObjects();
                for (size_t l = 0; l < linkedObjects.size(); l++) {
                    linkedObjects[l]->render(camera, lamps, enableFog);
                }
            }
            // Rendering the cars + player.
            carRenderer->render(carModelMatrices, camera, lamps, enableFog);
        }

        if (enableText)
        {
            if (!playerCar->wasHit()) {
                // Updating text of distance score
                score = int(-distance);
                std::string score_text_str = "score: " + std::to_string(score);
                score_text->Update(score_text_str);
                // Updating text of time elapsed in-game
                auto score_elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - score_start_time).count();
                std::string time_text_str = "time: " + std::to_string(score_elapsed_time);
                time_text->Update(time_text_str);
            }
            // Updating text of current speed
            int speed = -(playerCar->getRigidBody()->getLinearVelocity().getZ());
            std::string speed_text_str = "speed: " + std::to_string(speed);
            speed_text->Update(speed_text_str);
            // Drawing texts
            score_text->Draw(fontShader);
            speed_text->Draw(fontShader);
            time_text->Draw(fontShader);

            // Highscore update system
            bool k_shifted = false;
            std::string top_t;
            int h = FindHighScorePosition(score);
            if (h < best_scores_textes.size())
            {
                std::string top_t = playerName + "    " + std::to_string(score);
                best_scores_textes[h]->Update(top_t);
                best_scores_textes[h]->Draw(fontShader, true);
            }
            size_t k = 0;
            for (size_t t = 0; t < best_scores_textes.size(); t++)
            {
                if (h != t) {
                    top_t = highScores[t].name + "    " + std::to_string(highScores[k++].score);
                    best_scores_textes[t]->Update(top_t);
                    best_scores_textes[t]->Draw(fontShader);
                }
            }
        }

        // Framerate capping.
        //auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
        //int remainingTime = frameDelay - elapsedTime;
        //if (remainingTime > 0) {
        //    std::this_thread::sleep_for(std::chrono::milliseconds(remainingTime));
        //}
        //fps(glfwGetTime());

        // Buffer swap and others.
        glfwSwapBuffers(window->getWindow());
        glfwPollEvents();
    }
    UpdateHighScores(playerName, score);
    WriteHighScores();

    return 0;
}



//double lastF = 0;
//int deltaF = 0;
////fps function
//auto fps = [&](double now) {
//    double deltaT = now - lastF;
//    deltaF++;
//    if (deltaT > 0.5) {
//        lastF = now;
//        const double fpsCount = (double)deltaF / deltaT;
//        deltaF = 0;
//        std::cout << "\r FPS: " << fpsCount;
//    }
//};






// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
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
        if (processMouseInput) { processMouseInput = false; glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); std::cout << "\nMOUSE DETACHED\n"; }
        else { processMouseInput = true; glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); std::cout << "\nMOUSE ATTACHED\n"; }
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if (pauseGame) { pauseGame = false; std::cout << ">> Game resumed" << std::endl; }
        else { pauseGame = true; std::cout << ">> Game paused" << std::endl; }
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

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (enableFog) { enableFog = false; std::cout << ">> FOG OFF" << std::endl; }
        else { enableFog = true; std::cout << ">> FOG ON" << std::endl; }
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        if (enableText) { enableText = false; std::cout << ">> TEXT OFF" << std::endl; }
        else { enableText = true; std::cout << ">> TEXT ON" << std::endl; }
    }

    if (key == GLFW_KEY_V && action == GLFW_PRESS)
    {
        if (enableSpeed) { enableSpeed = false; std::cout << ">> INCREASING SPEED OFF" << std::endl; }
        else { enableSpeed = true; std::cout << ">> INCREASING SPEED ON" << std::endl; }
    }



}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    //SCR_WIDTH = width;
    //SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}
