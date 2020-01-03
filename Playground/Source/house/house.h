#pragma once
#include <vector>
#include <time.h>
#include <math.h>
#include "../camera.h"

using namespace tinyobj;

class House {
private:
	GLint shape_size;
	Shape* s;
	mat4 view;
	GLint type;
	mat4 model;
	GLint color;
	int pos;
	int h;
public:
	void setPos(int p) {
		pos = p;
		vec3 trans;
		pos %= 40;
		switch (pos) {
		case 0:
			trans = vec3(4-0.45, 0.1+h, -4);
			break;
		case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
			trans = vec3(4-0.45, 0.15 + h, -3.8 + pos * 8.0f / 10.5);
			break;
		case 10:
			trans = vec3(4, 0 + h, 4);
			break;
		case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:
			trans = vec3(3.8 - (pos - 10) * 8.0f / 10.5, 0 + h, 4 - 0.45);
			break;
		case 20:
			trans = vec3(-4, 0 + h, 4);
			break;
		case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:
			trans = vec3(-4+0.45, 0 + h, 3.8 - (pos - 20) * 8.0f / 10.5);
			break;
		case 30:
			trans = vec3(-4, 0 + h, -4);
			break;
		case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:
			trans = vec3(-3.8 + (pos - 30) * 8.0f / 10.5, 0 + h, -4 + 0.45);
			break;
		default:
			break;
		}
		trans.y = 0.15 + 0.3*h;
		model = translate(mat4(), trans);
		model = scale(model, vec3(0.3));
	}
	int getPos() {
		return pos;
	}
	House() {

	}
	House(GLint idx, GLint h, GLint color) {
		this->color = color;
		this->h = h;
		setPos(idx);
		vector<shape_t> shapes;
		vector<material_t> materials;

		string err;

		LoadObj(shapes, materials, err, "house/Sphere.obj");
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
		init();
	}
	void init() {

	}
	void setView(mat4 view) {
		this->view = view;
	}
	
	void draw(GLuint program) {
		glEnable(GL_CULL_FACE);
		for (int i = 0; i < shape_size; i++) {
			glBindVertexArray(s[i].vao);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(glGetUniformLocation(program, "flag"), color);
			glUniformMatrix4fv(glGetUniformLocation(program, "um4mv"), 1, GL_FALSE, value_ptr(view * model));
			glDrawElements(GL_TRIANGLES, s[i].indexCount, GL_UNSIGNED_INT, 0);
		}
		glDisable(GL_CULL_FACE);
	}
	mat4 getModelMatrix() {
		return model;
	}
};

class Community {
public:
	Community() {

	}
	~Community() {

	}
	void draw(GLuint program) {
		for (House house:community) {
			house.setView(view);
			house.draw(program);
		}
	}
	void setView(mat4 view) {
		this->view = view;
	}
	void add(House h) {
		community.push_back(h);
	}
private:
	vector<House> community;
	mat4 view;

};

