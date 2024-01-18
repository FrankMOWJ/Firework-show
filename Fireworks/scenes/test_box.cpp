#include "test_box.h"

TestBox::TestBox()
{
	float offset = 1.0f;
	float boxVertices[] = {
		// positions          
		-offset,  offset, -offset,
		-offset, -offset, -offset,
		 offset, -offset, -offset,
		 offset, -offset, -offset,
		 offset,  offset, -offset,
		-offset,  offset, -offset,

		-offset, -offset,  offset,
		-offset, -offset, -offset,
		-offset,  offset, -offset,
		-offset,  offset, -offset,
		-offset,  offset,  offset,
		-offset, -offset,  offset,

		 offset, -offset, -offset,
		 offset, -offset,  offset,
		 offset,  offset,  offset,
		 offset,  offset,  offset,
		 offset,  offset, -offset,
		 offset, -offset, -offset,

		-offset, -offset,  offset,
		-offset,  offset,  offset,
		 offset,  offset,  offset,
		 offset,  offset,  offset,
		 offset, -offset,  offset,
		-offset, -offset,  offset,

		-offset,  offset, -offset,
		 offset,  offset, -offset,
		 offset,  offset,  offset,
		 offset,  offset,  offset,
		-offset,  offset,  offset,
		-offset,  offset, -offset,

		-offset, -offset, -offset,
		-offset, -offset,  offset,
		 offset, -offset, -offset,
		 offset, -offset, -offset,
		-offset, -offset,  offset,
		 offset, -offset,  offset
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindVertexArray(0);
	
	shader = new Shader(FileSystem::getPath("shaderFiles/test_box.vs"), FileSystem::getPath("shaderFiles/test_box.fs"));
}

TestBox::~TestBox()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete shader;
}

void TestBox::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	shader->use();
	glm::mat4 model1 = glm::translate(model, glm::vec3(-3.0f, -3.0f, -7.0f));
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
