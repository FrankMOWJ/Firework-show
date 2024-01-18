#pragma once
#ifndef SKY_BOX_H
#define SKY_BOX_H
#include<iostream>
#include<glad/glad.h>
#include<string>
#include<vector>
#include"../utils/shader.h"
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"../utils/texture.h"
#include"../utils/filesystem.h"
using namespace std;

class SkyBox {
public:
    unsigned int textureID;
    Shader* shader;
    unsigned int skyboxVAO, skyboxVBO;

    SkyBox(const vector<string>& textures_faces);

    void render(glm::mat4& view, glm::mat4& projection);
    ~SkyBox();

    void loadCubemap(vector<std::string> faces);
};
#endif // !SKY_BOX_H


