#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../util.h"

class Texture2D {
public:
	GLuint texture;
	Texture2D() {};
	void init(const char* imageStr) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		TextureData data = loadImage(imageStr);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
private:
	// load a png image and return a TextureData structure with raw data
	// not limited to png format. works with any image format that is RGBA-32bit
	TextureData loadImage(const char* const Filepath)
	{
		TextureData texture;
		int n;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc *data = stbi_load(Filepath, &texture.width, &texture.height, &n, 4);
		if (data != NULL)
		{
			texture.data = new unsigned char[texture.width * texture.height * 4 * sizeof(unsigned char)];
			memcpy(texture.data, data, texture.width * texture.height * 4 * sizeof(unsigned char));
			stbi_image_free(data);
		}
		return texture;
	}
};

#endif // !TEXTURE_H
