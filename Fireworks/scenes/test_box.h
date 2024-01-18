#ifndef TEST_BOX_H
#define TEST_BOX_H

#include<glad/glad.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"../utils/shader.h"
#include"../utils/filesystem.h"

class TestBox {
public:
	GLuint VAO, VBO;
	Shader* shader;

	TestBox();
	~TestBox();
	void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
};
#endif // !TEST_BOX_H
