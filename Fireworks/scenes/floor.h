#pragma once
#ifndef FLOOR_H
#define FLOOR_H
#include <glad/glad.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "../utils/shader.h"
#include "../stb_image.h"
#include<vector>
#include"../particleSystem/Firework.h"
#include"../utils/filesystem.h"
class Floor {
private:

	GLuint groundVBO, groundVAO, groundEBO;
	unsigned int groundtexture;
	Shader* shader;
	glm::vec3 Normal;

public:
	Floor();

	~Floor();

	void render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

};
#endif // !FLOOR_H




