
#pragma once

#include "util.h"
#include "camera.h"

using namespace glm;
using namespace std;
using namespace tinyobj;

class Map {
private:
	int shape_size;
	Shape* s;
	GLuint tex;
    mat4 view;
    GLfloat degree;
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
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)(shapes[i].mesh.positions.size() * sizeof(float)));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)((shapes[i].mesh.positions.size() * sizeof(float)) + (shapes[i].mesh.texcoords.size() * sizeof(float))));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glGenBuffers(1, &s[i].ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s[i].ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size() * sizeof(unsigned int), shapes[i].mesh.indices.data(), GL_STATIC_DRAW);
			s[i].materialId = shapes[i].mesh.material_ids[0];
			s[i].indexCount = static_cast<int>(shapes[i].mesh.indices.size());
		}
		TextureData tdata = load_png(tex_file);

		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        focus(0);
	}
    void setView(mat4 view) {
        this->view = view;
    }
    void focus(int index) {
        if(index<10) {
            degree = 270;
        }
    }
	void draw(GLuint program) {
		for (int i = 0; i < shape_size; i++) {
			glBindVertexArray(s[i].vao);
			glBindTexture(GL_TEXTURE_2D, tex);
            mat4 model = rotate(mat4(), radians(degree), vec3(0, 1, 0));
            glUniformMatrix4fv(glGetUniformLocation(program, "um4mv"), 1, GL_FALSE, value_ptr(view * model));
			glDrawElements(GL_TRIANGLES, s[i].indexCount, GL_UNSIGNED_INT, 0);
		}
	}
};
