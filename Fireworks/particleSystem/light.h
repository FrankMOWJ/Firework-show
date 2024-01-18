#ifndef LIGHT_H
#define LIGHT_H

#include"../utils/shader.h"
#include"../utils/camera.h"
#include<vector>
#include<cmath>
#include<iostream>
#include"../utils/filesystem.h"

class Light {
public:
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 lightColor;

	Shader* shaderLight;
	std::vector<float> coords;
	GLuint VAO, VBO;
	Light(glm::vec3 position_, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	virtual void update(float dt) {}
	~Light();
};

#endif // !LIGHT_H
