#include "../headers/Skybox.hpp"
Skybox::Skybox(){}
Skybox::Skybox(Sun* sun) 
{
    cubeMapModel = new Model("assets/objects/cube.obj");
    cubeMapShader = new Shader("code/shaders/skybox.vert", "code/shaders/skybox.frag");
    this->sun = sun;
    cubeMap = new Object(cubeMapModel, cubeMapShader, nullptr, sun);
    // cubeMapTexture;
    glGenTextures(1, &cubeMapTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::string pathToCubeMap = "assets/skybox/";
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
}

void Skybox::render(Camera* camera, std::vector<StreetLamp*> lamps, bool enableFog)
{
    cubeMapShader->use();
    cubeMapShader->setMat4("V", camera->getViewMatrix());
    cubeMapShader->setMat4("P", camera->getProjectionMatrix());
    cubeMapShader->setInt("cubemapTexture", 0);
    cubeMapShader->setBool("isNight", (this->sun)->isNight());
    cubeMapShader->setBool("enableFog", enableFog);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    cubeMap->render(camera, lamps, enableFog);
}


void Skybox::loadCubemapFace(const char* path, const GLenum& targetFace)
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