#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <headers/Game.hpp>
#include <headers/Container.hpp>
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

    // The object to render
    Container box = Container("code/shaders/vertexShader.vs", "code/shaders/fragmentShader.fs");

    box.configure_VAO_VBO();
    box.set_vertex_attributes_pointers(0,3,5,0);
    box.set_vertex_attributes_pointers(1,2,5,3);

    // render loop
    while (!glfwWindowShouldClose(game.getWindow()))
    {
        // input
        processInput(game.getWindow());

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // render
        box.render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(game.getWindow());
        glfwPollEvents();
    }

    box.terminate();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    game.terminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}