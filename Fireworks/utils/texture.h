#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>

#include <iostream>
#include"../stb_image.h"

class Texture2D {
public:
	GLuint ID;	// 纹理ID，由glGenTextures()生成

	GLuint Width, Height;
	GLuint InternalFormat;
	GLuint ImageFormat;

	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint FilterMin;
	GLuint FilterMax;
	unsigned char* _data;
	Texture2D();
	Texture2D(const char* texturePath);
	void Generate(GLuint width, GLuint height, unsigned char* data);
	void Bind() const;
	static GLuint loadTexture(const char* fileName);
	static GLuint loadJPGFile(const char* filename);
	static bool hasEnding(std::string const& fullString, std::string const& ending);
};
#endif // !TEXTURE_H
