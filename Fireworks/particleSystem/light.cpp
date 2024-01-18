#include"light.h"
#define PI acos(-1)

Light::Light(glm::vec3 position_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :position(position_) {
    shaderLight = new Shader(FileSystem::getPath("shaderFiles/lightcube.vs"), FileSystem::getPath("shaderFiles/lightcube.fs"));
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;

    float size = 0.05;
    float vertices[] = {
        // back face
        -size, -size, -size,
         size,  size, -size,
         size, -size, -size,
         size,  size, -size,
        -size, -size, -size,
        -size,  size, -size,
        // front face
        -size, -size,  size,
         size, -size,  size,
         size,  size,  size,
         size,  size,  size,
        -size,  size,  size,
        -size, -size,  size,
        // left face
        -size,  size,  size,
        -size,  size, -size,
        -size, -size, -size,
        -size, -size, -size,
        -size, -size,  size,
        -size,  size,  size,
        // right face
         size,  size,  size,
         size, -size, -size,
         size,  size, -size,
         size, -size, -size,
         size,  size,  size,
         size, -size,  size,
         // bottom face
         -size, -size, -size,
          size, -size, -size,
          size, -size,  size,
          size, -size,  size,
         -size, -size,  size,
         -size, -size, -size,
         // top face
         -size,  size, -size,
          size,  size , size,
          size,  size, -size,
          size,  size,  size,
         -size,  size, -size,
         -size,  size,  size,
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    constant = 1.0;
    linear = 0.09;
    quadratic = 0.032;

    lightColor = diffuse;

    // 渲染圆环
    //float Rinner = 0.2f;//内圆半径
    //float Rring = 0.3f;//环半径
    //int count = 20;//循环次数
    //float alpha = 0;//外圆角度
    //float x0, x1, y0, y1, z0, z1;
    //float alphaStep = (float)(2 * PI / count);//alpha角的步长（外部圆环）
    //float betaStep = (float)(2 *PI / count);//beta角的步长（每个切片）
    //float beta = 0;//切片循环角度
    //int count0 = 20;//循环次数
    ///***************************外层循环主要负责把圆环切成几个部分*******************************/
    //for (int i = 0; i < count; i++) {
    //    alpha = i * alphaStep;
    //    /***************************内层循环主要负责把每个部分画出来*******************************/

    //    for (int j = 0; j <= count0; j++) {
    //        beta = j * betaStep;
    //        x0 = (float)(glm::cos(alpha) * (Rring + Rinner + Rinner * glm::cos(beta)));
    //        y0 = (float)(glm::sin(alpha) * (Rring + Rinner + Rinner * glm::cos(beta)));
    //        z0 = (float)-(Rring * glm::sin(beta));

    //        x1 = (float)(glm::cos(alpha + alphaStep) * (Rring + Rinner + Rinner * glm::cos(beta)));
    //        y1 = (float)(glm::sin(alpha + alphaStep) * (Rring + Rinner + Rinner * glm::cos(beta)));
    //        z1 = (float)(-Rring * glm::sin(beta));

    //        coords.push_back(x0);
    //        coords.push_back(y0);
    //        coords.push_back(z0);
    //        coords.push_back(x1);
    //        coords.push_back(y1);
    //        coords.push_back(z1);
    //    }
    //}
    //float* torus_vertices = new float[coords.size()];
    //memcpy(torus_vertices, &coords[0], coords.size() * sizeof(float));
    //glGenVertexArrays(1, &this->VAO);
    //glGenBuffers(1, &this->VBO);
    //glBindVertexArray(this->VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(torus_vertices), torus_vertices, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    //glEnableVertexAttribArray(0);
    //glBindVertexArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

}
extern Camera camera;
void Light::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    glBindVertexArray(this->VAO);
    shaderLight->use();
    shaderLight->setMat4("model", model);
    shaderLight->setMat4("view", view);
    shaderLight->setMat4("projection", projection);
    shaderLight->setVec3("lightColor", lightColor);
    shaderLight->setFloat("light.constant", constant);
    shaderLight->setFloat("light.linear", linear);
    shaderLight->setFloat("light.quadratic", quadratic);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDrawArrays(GL_LINE_STRIP, 0, coords.size() / 3);
}

Light::~Light() 
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    delete shaderLight;
}
