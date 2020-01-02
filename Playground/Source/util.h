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
#endif 

