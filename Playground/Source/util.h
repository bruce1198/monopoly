#ifndef UTIL_H
#define UTIL_H
#include "../Include/Common.h"
#include <vector>
#include <time.h>
#include <math.h>
#define SHADOW_MAP_SIZE 4096

using namespace glm;
using namespace std;

struct P {
	vec3 pos;
	vec3 degree;
	GLfloat yaw;
	GLfloat pitch;
};
struct A {
	vec3 pos;
	vec3 degree;
	GLfloat yaw;
	GLfloat pitch;
};
struct V {
	vec3 pos;
	vec3 degree;
	GLfloat yaw;
	GLfloat pitch;
};

typedef struct
{
	GLuint vao;            // vertex array object
	GLuint vbo;            // vertex buffer object
	GLuint vboTex;        // vertex buffer object of texture
	GLuint ebo;

	GLuint p_normal;
	int materialId;
	int indexCount;
	GLuint m_texture;
} Shape;

struct Fbo {
	GLuint fbo;
	GLuint rbo;
	GLuint texture;
};

void print(vec3 target) {
	cout << "(" << target.x << ", " << target.y << ", " << target.z << ")" << endl;
}

class Shader {
private:
	GLuint program;
	char** loadShaderSource(const char* file)
	{
		FILE* fp = fopen(file, "rb");
		fseek(fp, 0, SEEK_END);
		long sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char *src = new char[sz + 1];
		fread(src, sizeof(char), sz, fp);
		src[sz] = '\0';
		char **srcp = new char*[1];
		srcp[0] = src;
		return srcp;
	}
	void freeShaderSource(char** srcp)
	{
		delete srcp[0];
		delete srcp;
	}
public:
	Shader() {

	}
	Shader(const char* vs, const char* fs) {
		program = glCreateProgram();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		char** vertexShaderSource = loadShaderSource(vs);
		char** fragmentShaderSource = loadShaderSource(fs);

		// Assign content of these shader files to those shaders we created before
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		// Free the shader file string(won't be used any more)
		freeShaderSource(vertexShaderSource);
		freeShaderSource(fragmentShaderSource);

		// Compile these shaders
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		shaderLog(vertexShader);
		shaderLog(fragmentShader);

		// Assign the program we created before with these shaders
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
	}
	void use() {
		glUseProgram(program);
	}
	GLuint getProgram() { return program; }
};
class Skybox {
private:
	GLuint cubemapTexture;
	GLuint skyboxVAO, skyboxVBO;
	float skyboxVertices[108] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
public:
	Skybox() {

	}
	Skybox(vector<const char*> faces) {

		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glGenTextures(1, &cubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		int width, height, nrChannels;
		unsigned char* image;
		for (int i = 0; i < faces.size(); i++) {
			stbi_set_flip_vertically_on_load(true);
			unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
			if (data != NULL) {
				image = new unsigned char[width*height*nrChannels * sizeof(unsigned char)];
				memcpy(image, data, width*height*nrChannels * sizeof(unsigned char));
				stbi_image_free(data);
			}
			glBindTexture(GL_TEXTURE_2D, cubemapTexture);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
	}
	void draw() {
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_2D, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
};
#endif 

