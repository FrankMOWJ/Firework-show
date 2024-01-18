#include"sky_box.h"



SkyBox::SkyBox(const vector<string>& textures_faces) {
	float offset = 50.0f;		// 不能超过100.0，应为这是投影的最末端
	float skyboxVertices[] = {
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
	this->loadCubemap(textures_faces);

	shader = new Shader(FileSystem::getPath("shaderFiles/sky_box.vs"), FileSystem::getPath("shaderFiles/sky_box.fs"));

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	shader->use();
	shader->setInt("skybox", 0);
}

void SkyBox::render(glm::mat4& view, glm::mat4& projection) {
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shader->use();
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

SkyBox::~SkyBox() {
	delete shader;
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
}

void SkyBox::loadCubemap(vector<std::string> faces)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
