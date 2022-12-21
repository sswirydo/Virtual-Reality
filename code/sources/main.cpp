#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <headers/Game.hpp>
#include <headers/camera.hpp>
#include <headers/Object.hpp>
#include <iostream>

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // openGl initialisation
    Game game;
    try
    {
        game.initOpenGL("test",SCR_WIDTH,SCR_HEIGHT);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        game.terminate();
        exit(EXIT_FAILURE);
    }
    Shader shader = Shader("code/shaders/vertSrc.vs","code/shaders/fragSrc.fs");
    std::cout <<"1" << '\n';
    Model model = Model("assets/meshes/car/source/TESLA_last_withoutinside_3.fbx");
    std::cout << "2" << '\n';
    Object bag = Object(model,shader);
    std::cout << "3"<< '\n';
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    std::cout << "4"<< '\n';

    // render loop
    while (!glfwWindowShouldClose(game.getWindow()))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(game.getWindow());

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        shader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        bag.render();


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(game.getWindow());
        glfwPollEvents();
    }
    game.terminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}