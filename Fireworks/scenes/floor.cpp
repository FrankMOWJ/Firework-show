#include "floor.h"

Floor::Floor()
{
	GLfloat offset = 50.0f;
	float groundVertices[] = {
		// positions            // texture coordinates // Normal
		-offset, 0.0f, -offset,     0.0f, 0.0f,			0.0, 1.0, 0.0,
		 offset, 0.0f, -offset,     1.0f, 0.0f,			0.0, 1.0, 0.0,
		 offset, 0.0f,  offset,     1.0f, 1.0f,			0.0, 1.0, 0.0,
		-offset, 0.0f,  offset,     0.0f, 1.0f,			0.0, 1.0, 0.0
	};
	Normal = glm::vec3(0.0, 1.0, 0.0);
	GLuint groundIndices[] = {
		0, 1, 2,
		2, 3, 0
	};
	
	shader = new Shader(FileSystem::getPath("shaderFiles/floor.vs"), FileSystem::getPath("shaderFiles/floor.fs"));
	printf("load shader file successfully\n");

	glGenVertexArrays(1, &groundVAO);
	glGenBuffers(1, &groundVBO);
	glGenBuffers(1, &groundEBO);

	glBindVertexArray(groundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	unsigned char* data;
	glGenTextures(1, &groundtexture);
	glBindTexture(GL_TEXTURE_2D, groundtexture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	// stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	int width, height, nrChannels;
	std::string floor_texture_path = FileSystem::getPath("floor.jpg");
	data = stbi_load(floor_texture_path.c_str(), &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(false);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture of ground" << std::endl;
	}
	stbi_image_free(data);

	shader->use();
	shader->setInt("groundtexture", 0);
}

Floor::~Floor()
{
	delete shader;
	glDeleteBuffers(1, &groundVBO);
	glDeleteVertexArrays(1, &groundVAO);
	glDeleteBuffers(1, &groundEBO);
}

extern std::vector<Firework*> _fireworks;
extern Camera camera;

void Floor::render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	shader->use();
	shader->setVec3("viewPos", camera.Position);
	int k = 0;
	for (int i = 0; i < _fireworks.size(); i++) {
		if (_fireworks[i]->light == nullptr)continue;

		shader->setVec3("pointLights[" + std::to_string(k) + "].position", _fireworks[i]->light->position);
		shader->setVec3("pointLights[" + std::to_string(k) + "].ambient", _fireworks[i]->light->ambient);
		shader->setVec3("pointLights[" + std::to_string(k) + "].diffuse", _fireworks[i]->light->diffuse);
		shader->setVec3("pointLights[" + std::to_string(k) + "].specular", _fireworks[i]->light->specular);
		shader->setFloat("pointLights[" + std::to_string(k) + "].constant", _fireworks[i]->light->constant);
		shader->setFloat("pointLights[" + std::to_string(k) + "].linear", _fireworks[i]->light->linear);
		shader->setFloat("pointLights[" + std::to_string(k) + "].quadratic", _fireworks[i]->light->quadratic);
		k++;
	}
	while (k < 10) {
		shader->setVec3("pointLights[" + std::to_string(k) + "].position", glm::vec3(1, 1, 1));
		shader->setVec3("pointLights[" + std::to_string(k) + "].ambient", glm::vec3(0, 0, 0));
		shader->setVec3("pointLights[" + std::to_string(k) + "].diffuse", glm::vec3(0, 0, 0));
		shader->setVec3("pointLights[" + std::to_string(k) + "].specular", glm::vec3(0, 0, 0));
		shader->setFloat("pointLights[" + std::to_string(k) + "].constant", 1);
		shader->setFloat("pointLights[" + std::to_string(k) + "].linear", 1);
		shader->setFloat("pointLights[" + std::to_string(k) + "].quadratic", 1);
		k++;
	}

	model = glm::translate(model, glm::vec3(0, -0.0f, 0));
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	// 绑定地面纹理
	glBindVertexArray(groundVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundtexture);
	// 绘制地面
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// 解绑VAO和纹理
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
