#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <headers/Game.hpp>
#include <headers/camera.hpp>
#include <headers/Object.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800*1.5; // 800x600 ? are you executing this on your phone or what ? :p
const unsigned int SCR_HEIGHT = 600*1.5;

// camera
Camera camera(glm::vec3(0.0f, 2.7f, 4.9f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool processMouseInput = true;

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
    glfwSetFramebufferSizeCallback(game.getWindow(), framebuffer_size_callback);
    glfwSetCursorPosCallback(game.getWindow(), mouse_callback);
    glfwSetScrollCallback(game.getWindow(), scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shader = Shader("code/shaders/vertSrc.vert","code/shaders/fragSrc.frag");
    std::cout <<"1" << '\n';
    Model model = Model("assets/meshes/alpha/AS5QG9E1JE65KQEOKSS4QB8ON.obj");
    std::cout << "2" << '\n';
    Object car = Object(model,shader);
    std::cout << "3"<< '\n';
    std::cout << "4"<< '\n';

    // TEST
    // ----------------------------------------
    // Adapted from:
    // https://www.dropbox.com/s/47qk4yrz5v9lb61/Terrain%20Generation%20Code.txt?dl=0
    std::vector<Vertex> terrainVertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int vertex_count = 20;
    int size = 40;
    int count = vertex_count * vertex_count;

    Vertex terrain_vertex;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoords;

    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < vertex_count; j++) {
            position.x = (float)j / ((float)vertex_count - 1) * size;
            position.y = 0;
            position.z = (float)i / ((float)vertex_count - 1) * size;
            position.x -= (float)size / 2; // translate for center (temp)
            position.z -= (float)size / 2;
            normal.x = 0;
            normal.y = 1;
            normal.z = 0;
            texCoords.x = (float)j / ((float)vertex_count - 1);
            texCoords.y = (float)i / ((float)vertex_count - 1);
            terrain_vertex.Position = position;
            terrain_vertex.Normal = normal;
            terrain_vertex.TexCoords = texCoords;
            terrainVertices.push_back(terrain_vertex);
        }
    }
    for (int gz = 0; gz < vertex_count - 1; gz++) {
        for (int gx = 0; gx < vertex_count - 1; gx++) {
            int topLeft = (gz * vertex_count) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * vertex_count) + gx;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    Mesh terrainMesh = Mesh(terrainVertices, indices, textures);
    // TEST END
    // ----------------------------------------


    glfwSetKeyCallback(game.getWindow(), key_callback);

    double prev = 0;
    int deltaFrame = 0;
    //fps function
    auto fps = [&](double now) {
        double deltaTime = now - prev;
        deltaFrame++;
        if (deltaTime > 0.5) {
            prev = now;
            const double fpsCount = (double)deltaFrame / deltaTime;
            deltaFrame = 0;
            std::cout << "\r FPS: " << fpsCount;
        }
    };


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

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 8000.0f);
        shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        // render the loaded model 
        glm::mat4 model = glm::mat4(1.0f);
        // On retourne la voiture de 90° suivant l'axe Y, vu que de base elle regardait vers la droite
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));                                                                           
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));	// On l'a ramene un peu vers le haut vu qu'elle etait trop basse
        shader.setMat4("model", model);

        car.render();

        terrainMesh.Draw(shader);

        fps(glfwGetTime());


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(game.getWindow());
        glfwPollEvents();
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