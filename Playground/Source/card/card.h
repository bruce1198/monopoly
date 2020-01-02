#pragma once
#include <vector>
#include <time.h>
#include <math.h>
#include "../camera.h"

using namespace tinyobj;

class Card {
private:
	GLint shape_size;
	Shape* s;
	GLuint face;
	GLuint back;
    mat4 view;
    GLfloat degree;
	GLint type;
	mat4 model;
	GLint idx;
	GLint h;
	bool wait;
	bool waitdiscard;
	bool slide;
	bool gocent;
	bool discard;
	bool isdiscarded;
	GLfloat cnt;
	A a;
	V v;
	P p;
	GLint timer;
	GLint timeout;
public:
	Card() {

	}
	Card(int idx, int type) {
		this->type = type;
		if(type==0)
			this->idx = idx-3;
		else if(type==1)
			this->idx = idx - 23;
		vector<shape_t> shapes;
		vector<material_t> materials;

		string err;

		LoadObj(shapes, materials, err, "card/Plane.obj");
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
		string s = string("card/" + to_string(idx) + ".png");
		TextureData tdata = load_png(s.c_str(), false);

		glGenTextures(1, &face);
		glBindTexture(GL_TEXTURE_2D, face);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if(type==0)
			tdata = load_png("card/1.png", true);
		else
			tdata = load_png("card/2.png", true);

		glGenTextures(1, &back);
		glBindTexture(GL_TEXTURE_2D, back);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		init();
	}
	void init() {
		if (type == 0) {
			p.pos = vec3(1.83, 0.1, 1.87);
		}
		else {
			p.pos = vec3(-1.4, 0.1, -1.66);
		}
		p.degree.x = 0.0f;
		p.degree.y = -45.0f;
		p.degree.z = 0.0f;
		slide = false;
		gocent = false;
		discard = false;
		isdiscarded = false;
		wait = false;
		waitdiscard = false;
	}
    void setView(mat4 view) {
        this->view = view;
    }
	bool update() {
		bool status = false;
		if (timer > timeout && wait) {
			slide = true;
			wait = false;
			vec3 to = type == 0 ? vec3(1.83 + 1, 0.1 + 0.01*h, 1.87 - 1) : vec3(-1.4 - 1, 0.1 + 0.01*h, -1.66 + 1);
			cnt = 0;
			a.pos = (to - p.pos) / vec3(1250);
			v.pos = vec3(50)*a.pos;
		}
		else if (wait) {
			timer++;
		}
		if (slide) {
			if (cnt <= 50) {
				p.pos += v.pos;
				v.pos -= a.pos;
				cnt++;
			}
			else {
				slide = false;
				gocent = true;
				vec3 to = vec3(-6, 3, 0);
				cnt = 0;
				a.pos = (to - p.pos) / vec3(1250);
				a.degree.x = (-90 - p.degree.x) / 1250;
				a.degree.y = (-90 - p.degree.y) / 1250;
				a.degree.z = (-30 - p.degree.z) / 1250;
				v.pos = vec3(50)*a.pos;
				v.degree = vec3(50) * a.degree;
			}
		}
		else if (gocent) {
			if (cnt <= 50) {
				p.pos += v.pos;
				p.degree += v.degree;
				v.pos -= a.pos;
				v.degree -= a.degree;
				cnt++;
				if (v.pos.x >= 0 && p.pos.x >= -6 || v.pos.x <= 0 && p.pos.x <= -6) {
					p.pos.x = -6;
				}
				if (v.pos.y >= 0 && p.pos.y >= 3 || v.pos.y <= 0 && p.pos.y <= 3) {
					p.pos.y = 3;
				}
				if (v.pos.z >= 0 && p.pos.z >= 0 || v.pos.z <= 0 && p.pos.z <= 0) {
					p.pos.z = 0;
				}
				if (v.degree.x >= 0 && p.degree.x >= -90 || v.degree.x <= 0 && p.degree.x <= -90) {
					p.degree.x = -90;
				}
				if (v.degree.y >= 0 && p.degree.y >= -90 || v.degree.y <= 0 && p.degree.y <= -90) {
					p.degree.y = -90;
				}
				if (v.degree.z >= 0 && p.degree.z >= -30 || v.degree.z <= 0 && p.degree.z <= -30) {
					p.degree.z = -30;
				}
			}
			else {
				gocent = false;
				timer = 0;
				timeout = 50;
				waitdiscard = true;
			}
		}
		if (timer > timeout && waitdiscard) {
			waitdiscard = false;
			discard = true;
			vec3 to = vec3(-6, 3, 8);
			cnt = 0;
			a.pos = (to - p.pos) / vec3(1250);
			v.pos = vec3(50)*a.pos;
		}
		else if (waitdiscard) {
			timer++;
		}
		else if (discard) {
			if (cnt <= 50) {
				p.pos += v.pos;
				v.pos -= a.pos;
				cnt++;
			}
			else {
				status = true;
				discard = false;
				isdiscarded = true;
				cnt = 0;
			}
		}
		updateModel();
		return status;
	}
	void getCard() {
		wait = true;
		timer = 0;
		timeout = 50;
	}
	void draw(GLuint program) {
		glEnable(GL_CULL_FACE);
		for (int i = 0; i < shape_size; i++) {
			glBindVertexArray(s[i].vao);
			glBindTexture(GL_TEXTURE_2D, face);
			glCullFace(GL_FRONT);
            glUniformMatrix4fv(glGetUniformLocation(program, "um4mv"), 1, GL_FALSE, value_ptr(view * model));
			glDrawElements(GL_TRIANGLES, s[i].indexCount, GL_UNSIGNED_INT, 0);
			glBindTexture(GL_TEXTURE_2D, back);
			glCullFace(GL_BACK);
			glDrawElements(GL_TRIANGLES, s[i].indexCount, GL_UNSIGNED_INT, 0);
		}
		glDisable(GL_CULL_FACE);
	}
	void updateModel() {
		model = translate(mat4(), p.pos); 
		//modification
		mat4 x_bias = rotate(mat4(), -radians(p.degree.x), vec3(1.0, 0.0, 0.0));
		//y, z
		vec3 vec_y = normalize(vec3(x_bias * vec4(0.0, 1.0, 0.0, 1.0)));
		vec3 vec_z = normalize(vec3(x_bias * vec4(0.0, 0.0, 1.0, 1.0)));
		//z
		mat4 y_bias = rotate(mat4(), -radians(p.degree.y), vec_y);
		vec_z = normalize(vec3(y_bias * vec4(vec_z, 1.0)));
		//vec_z = normalize(vec3(y_bias * vec4(0.0, 0.0, 1.0, 1.0)));

		model = rotate(model, radians(p.degree.x), vec3(1.0, 0.0, 0.0));
		model = rotate(model, radians(p.degree.y), vec_y);
		model = rotate(model, radians(p.degree.z), vec_z);

		model = scale(model, vec3(0.13, 0.255, 0.255));
	}
	mat4 getModelMatrix() {
		return model;
	}
	bool isDiscard() {
		return isdiscarded;
	}
	void setH(GLint h) {
		this->h = h;
		p.pos.y = 0.1 + 0.01*h;
	}
	GLint getH() {
		return h;
	}
};

class CardGroup {
public:
	CardGroup() {

	}
	CardGroup(int size) {
		this->size = size;
		for (int i = 0; i < size; i++) {
			Card c = Card(i + 3, 0);
			c.setH(i);
			chance.push_back(c);
			c = Card(i + 23, 1);
			c.setH(i);
			fate.push_back(c);
		}
		c_remain = 20;
		f_remain = 20;
	}
	~CardGroup() {

	}
	void draw(GLuint program) {
		for (int i = 0; i < size; i++) {
			if (!fate[i].isDiscard()) {
				fate[i].setView(view);
				fate[i].draw(program);
			}
			if (!chance[i].isDiscard()) {
				chance[i].setView(view);
				chance[i].draw(program);
			}
		}
	}
	void setView(mat4 view) {
		this->view = view;
	}
	bool update() {
		bool dis = false;
		for (Card& c : chance) {
			dis |= c.update();
		}
		for (Card& f : fate) {
			dis |= f.update();
		}
		return dis;
	}
	void getCard(int type, int idx) {
		int id = idx % size;
		if (type == 0) { // opp
			cout << "go get opportunity card " << id << endl;
			for (Card &c : chance) {
				if (c.getH() == c_remain) {
					c.setH(id);
					chance[id].setH(c_remain);
				}
			}
			chance[id].getCard();
			c_remain--;
		}
		else if (type == 1) {// fate
			cout << "go get fate card " << id << endl;
			for (Card &f : fate) {
				if (f.getH() == f_remain-1) {
					f.setH(id);
					fate[id].setH(f_remain-1);
				}
			}
			fate[id].getCard();
			f_remain--;
		}

	}
private:
	GLint size;
	GLint c_remain;
	GLint f_remain;
	vector<Card> chance;
	vector<Card> fate;
	mat4 view;

};

