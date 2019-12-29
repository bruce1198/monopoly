#pragma once
#include "../Include/Common.h"
#include <vector>
#include <time.h>
#include <math.h>

using namespace glm;
using namespace std;
using namespace tinyobj;

// Load shader file to program
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

void print(vec3 target) {
	cout << "("<<target.x << ", " << target.y << ", "<<target.z << ")"<<endl;
}

// Free shader file
void freeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}
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

class Map {
private:
	int shape_size;
	Shape* s;
	GLuint tex;
public:
	Map() {

	}
	Map(const char* file, const char* tex_file) {
		vector<shape_t> shapes;
		vector<material_t> materials;

		string err;

		LoadObj(shapes, materials, err, file);
		if (err.size() > 0) {
			printf("Load Models Fail! Please check the solution path");
			return;
		}
		shape_size = (int)shapes.size();
		s = new Shape[shape_size];
		for (int i = 0; i < shape_size; i++) {
			glGenVertexArrays(1, &s[i].vao);
			glBindVertexArray(s[i].vao);

			glGenBuffers(1, &s[i].vbo);

			glBindBuffer(GL_ARRAY_BUFFER, s[i].vbo);
			glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float) + shapes[i].mesh.texcoords.size() * sizeof(float) + shapes[i].mesh.normals.size() * sizeof(float), NULL, GL_STATIC_DRAW);

			glBufferSubData(GL_ARRAY_BUFFER, 0, shapes[i].mesh.positions.size() * sizeof(float), &shapes[i].mesh.positions[0]);
			glBufferSubData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float), shapes[i].mesh.texcoords.size() * sizeof(float), &shapes[i].mesh.texcoords[0]);
			glBufferSubData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size() * sizeof(float) + shapes[i].mesh.texcoords.size() * sizeof(float), shapes[i].mesh.normals.size() * sizeof(float), &shapes[i].mesh.normals[0]);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)((shapes[i].mesh.positions.size() * sizeof(float)) + (shapes[i].mesh.texcoords.size() * sizeof(float))));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)(shapes[i].mesh.positions.size() * sizeof(float)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glGenBuffers(1, &s[i].ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s[i].ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), shapes[i].mesh.indices.data(), GL_STATIC_DRAW);
			s[i].materialId = shapes[i].mesh.material_ids[0];
			s[i].indexCount = static_cast<int>(shapes[i].mesh.indices.size());
		}
		texture_data tdata = load_png(tex_file);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void draw() {
		for (int i = 0; i < shape_size; i++) {
			glBindVertexArray(s[i].vao);
			glDrawElements(GL_TRIANGLES, s[i].indexCount, GL_UNSIGNED_INT, 0);
		}
	}
};