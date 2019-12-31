#pragma once

#ifndef SHADER_H
#define SHADER_H

class Shader {
public:
	Shader() {};

	GLuint program;
	
	void init(const char* vertexPath, const char* fragmentPath) {
		program = glCreateProgram();
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			char** vertexShaderSource = loadShaderSource(vertexPath);
			char** fragmentShaderSource = loadShaderSource(fragmentPath);

			glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
			glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

			freeShaderSource(vertexShaderSource);
			freeShaderSource(fragmentShaderSource);
			glCompileShader(vertexShader);
			glCompileShader(fragmentShader);

			shaderLog(vertexShader);
			shaderLog(fragmentShader);

			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
		glLinkProgram(program);
	}
private:
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
		delete[] srcp[0];
		delete[] srcp;
	}

	void shaderLog(GLuint shader)
	{
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			GLchar* errorLog = new GLchar[maxLength];
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			printf("%s\n", errorLog);
			delete[] errorLog;
		}
	}
};


#endif // ! SHADER_H