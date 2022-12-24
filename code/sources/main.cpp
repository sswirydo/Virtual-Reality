#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <headers/Game.hpp>
#include <headers/camera.hpp>
#include <headers/Object.hpp>
#include <headers/terrain.hpp>

void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void loadCubemapFace(const char* path, const GLenum& targetFace);

#ifndef NDEBUG
void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
#endif


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

#ifndef NDEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif

    glfwSetFramebufferSizeCallback(game.getWindow(), framebuffer_size_callback);
    glfwSetCursorPosCallback(game.getWindow(), mouse_callback);
    glfwSetScrollCallback(game.getWindow(), scroll_callback);
    glfwSetKeyCallback(game.getWindow(), key_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(game.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shader = Shader("code/shaders/car.vert","code/shaders/car.frag");
    Model model = Model("assets/meshes/alpha/AS5QG9E1JE65KQEOKSS4QB8ON.obj");
    Object car = Object(model,shader);

    Object terrain = generateTerrain();

    // SKYBOX TEST _ START
    Model cubeMapModel("assets/objects/cube.obj");
    Shader cubeMapShader = Shader("code/shaders/skybox.vert", "code/shaders/skybox.frag");
    Object cubeMap = Object(cubeMapModel, cubeMapShader);
    GLuint cubeMapTexture;
    glGenTextures(1, &cubeMapTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::string pathToCubeMap = PATH_TO_PROJECT "/assets/skybox/";
    std::cout << pathToCubeMap << std::endl;
    std::map<std::string, GLenum> facesToLoad = {
        {pathToCubeMap + "right.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_X},
        {pathToCubeMap + "top.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
        {pathToCubeMap + "front.jpg",GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
        {pathToCubeMap + "left.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
        {pathToCubeMap + "bottom.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
        {pathToCubeMap + "back.jpg",GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
    };
    //load the six faces
    for (std::pair<std::string, GLenum> pair : facesToLoad) {
        loadCubemapFace(pair.first.c_str(), pair.second);
    }
    // SKYBOX TEST _ END


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

    glm::mat4 perspective = camera.GetProjectionMatrix();


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

        glDepthFunc(GL_LEQUAL);
        car.render();
        terrain.render();

        // cubemap -- test
        cubeMapShader.use();
        cubeMapShader.setMat4("V", view);
        cubeMapShader.setMat4("P", perspective);
        cubeMapShader.setInt("cubemapTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        cubeMap.render();
        glDepthFunc(GL_LESS);
        // cubemap -- test



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


void loadCubemapFace(const char* path, const GLenum& targetFace)
{
    int imWidth, imHeight, imNrChannels;
    unsigned char* data = stbi_load(path, &imWidth, &imHeight, &imNrChannels, 0);
    if (data)
    {

        glTexImage2D(targetFace, 0, GL_RGB, imWidth, imHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(targetFace);
    }
    else {
        std::cout << "Failed to Load texture" << std::endl;
        const char* reason = stbi_failure_reason();
        std::cout << reason << std::endl;
    }
    stbi_image_free(data);
}