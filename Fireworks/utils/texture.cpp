#include "texture.h"


Texture2D::Texture2D()
	: Width(0), Height(0), InternalFormat(GL_RGB), ImageFormat(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT),FilterMin(GL_LINEAR), FilterMax(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
	this->_data = nullptr;
}

Texture2D::Texture2D(const char* texturePath) {
	int w, h, c;
	this->_data = stbi_load(texturePath, &w, &h, &c, 0);
	this->Width = w, this->Height = h;
}
void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;

	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, this->ImageFormat,GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

//Returns an opengl texture from the passed file, or -1 if not sucessful
GLuint Texture2D::loadTexture(const char* fileName) {

	//create a string from the file name
	std::string temp;
	temp.assign(fileName);
	if (Texture2D::hasEnding(fileName, ".jpg"))
		return Texture2D::loadJPGFile(fileName);
	return -1;
}


GLuint Texture2D::loadJPGFile(const char* filename)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: "<<filename << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

//checks to see if a passed string has the other parameter as the ending
bool Texture2D::hasEnding(std::string const& fullString, std::string const& ending)
{
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}
