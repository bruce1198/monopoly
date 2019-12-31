
#pragma once
#include "../util.h"

class Dice {
public:
	Dice() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), &vertex, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		int width, height, nrChannels;
		unsigned char* image;
		stbi_set_flip_vertically_on_load(false);
		for (unsigned int i = 0; i < faces.size(); i++) {
			stbi_uc* data = stbi_load(faces[i], &width, &height, &nrChannels, STBI_rgb_alpha);
			if (data) {
				image = new unsigned char[width*height * 4 * sizeof(unsigned char)];
				memcpy(image, data, width*height * 4 * sizeof(unsigned char));
				stbi_image_free(data);
				glBindTexture(GL_TEXTURE_2D, texture);
				if (nrChannels == 3)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
				else if (nrChannels == 4)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			}
		}
		scl = vec3(0.25);
		vec_x = vec3(1.0, 0.0, 0.0);
		vec_y = vec3(0.0, 1.0, 0.0);
		vec_z = vec3(0.0, 0.0, 1.0);
		rolling = false;
		moving = false;
	}

	~Dice() {}
	void setPos(vec3 pos) {
		this->pos = pos;
	}
	vec3 getPos() { return pos; }
	void roll() {
		int max = 24;
		int min = 15;
		//degree = mod(floor(degree),vec3(360));
		float xrand = floor(((float)rand() / RAND_MAX * 2)) * 90;
		float yrand = floor(((float)rand() / RAND_MAX * (max - min)) + min) * 90;
		float zrand = floor(((float)rand() / RAND_MAX * (5 - 1)) + 1) * 90;
		target = vec3(90, yrand, zrand);
		//target = vec3(90, 0, 0);
		cnt = 0;
		rolling = true;
	}
	int getPoints() {
		degree = mod(floor(degree), vec3(360));
		//print(degree);
		GLuint p = -1;
		if (degree == vec3(90, 0, 0) || degree == vec3(90, 180, 180))
			p = 6;
		else if (degree == vec3(90, 90, 0) || degree == vec3(90, 180, 270) || degree == vec3(90, 270, 180) || degree == vec3(90, 0, 270))
			p = 5;
		else if (degree == vec3(90, 270, 90) || degree == vec3(90, 90, 90))
			p = 4;
		else if (degree == vec3(90, 90, 270) || degree == vec3(90, 270, 270))
			p = 3;
		else if (degree == vec3(90, 270, 0) || degree == vec3(90, 0, 90) || degree == vec3(90, 90, 180) || degree == vec3(90, 180, 90))
			p = 2;
		else if (degree == vec3(90, 180, 0) || degree == vec3(90, 0, 180))
			p = 1;
		return p;
	}
	int update() {
		if (rolling) {
			if (cnt <= 50) {
				degree = target - vec3(pow(cnt - 50, 2)/2);
				cnt+=1;
			}
			else {
				rolling = false;
				return getPoints();
			}
		}
		if (moving) {
			if (move_cnt <= 50) {
				//pos += (to - from) / vec3(50);
				pos += v;
				v -= a;
				if (flag == 1) { //bigger
					scl += vec3(sv);
				}
				else if (flag == 2) { //smaller
					scl -= vec3(sv);
				}
				sv -= sa;
				move_cnt += 1;
			}
			else {
				moving = false;
			}
		}
		return -1;
	}
	void move(vec3 to) {
		this->from = pos;
		this->to = to;
		moving = true;
		move_cnt = 0;
		a =  (to-from)/vec3(1250);
		v = vec3(50)*a;
		sa = 0.25f/1250;
		sv = 50*sa;
		flag = 0;
	}
	void getStatus(bool*& status) {
		status[0] = moving;
		status[1] = rolling;
	}
	void moveWithScale(vec3 to, int flag) {
		move(to);
		this->flag = flag;
	}
	mat4 getModelMatrix() {
		mat4 matrix;
		matrix = translate(matrix, pos);
		matrix = scale(matrix, scl);

		//modification
		mat4 x_bias = rotate(mat4(), -radians(degree.x), vec3(1.0, 0.0, 0.0));
		//y, z
		vec_y = normalize(vec3(x_bias * vec4(0.0, 1.0, 0.0, 1.0)));
		vec_z = normalize(vec3(x_bias * vec4(0.0, 0.0, 1.0, 1.0)));
		//z
		mat4 y_bias = rotate(mat4(), -radians(degree.y), vec3(0.0, 1.0, 0.0));
		vec_z = normalize(vec3(y_bias * vec4(0.0, 0.0, 1.0, 1.0)));

		matrix = rotate(matrix, radians(degree.x), vec3(1.0, 0.0, 0.0));
		matrix = rotate(matrix, radians(degree.y), vec_y);
		matrix = rotate(matrix, radians(degree.z), vec_z);

		return matrix;
	}
	void draw() {
		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
private:
	//physics
	vec3 pos;
	vec3 scl;
	vec3 v;
	vec3 a;
	GLfloat sv;
	GLfloat sa;
	//roll
	vec3 degree;
	vec3 target;
	GLfloat cnt;
	bool rolling;
	vec3 vec_x;
	vec3 vec_y;
	vec3 vec_z;
	//move
	bool moving;
	vec3 from;
	vec3 to;
	GLfloat move_cnt;
	GLuint flag;
	//basics
	vector<const char*> faces = { "dice/one.png","dice/six.png", "dice/two.png", "dice/five.png", "dice/three.png", "dice/four.png" };
	GLuint texture;
	GLuint vao;
	GLuint vbo;
	GLfloat vertex[108] = {
		// Positions          
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
};
class DiceGroup {
public:
	DiceGroup() {

	}
	~DiceGroup() {

	}
	DiceGroup(int size) {
		this->size = size;
		srand((unsigned int)time(NULL));
		goup = false;
		rolling = false;
		godown = false;
		timer = 0;
		points = 0;
		pos = vec3(0, 0.35, 0.0);
		for (int i = 0; i < size; i++) {
			Dice d;
			float mid = 0.5;
			d.setPos(vec3(0.0, 0.35, (i - mid)*0.5));
			group.push_back(d);
		}
		dir = 0;
		timer = 0;
		timeout = 50;
		timer1 = 0;
		timeout1 = 30;
		wait = false;
		pleasegetpoint = false;
	}
	void draw(GLuint program) {
		for (int i = 0; i < size; i++) {
			mat4 model = group.at(i).getModelMatrix();
			glUniform1i(glGetUniformLocation(program, "flag"), i);
			glUniformMatrix4fv(glGetUniformLocation(program, "um4mv"), 1, GL_FALSE, value_ptr(view*model));
			group.at(i).draw();
		}
	}
	void roll(int timeout) {
		if (!goup && !rolling && !godown) {
			timer = 0;
			this->timeout = timeout;
			wait = true;
			pleasegetpoint = false;
		}
	}
	int* update() {
		int* dpoints = new int[2];
		bool* status = new bool[2];
		group.at(0).getStatus(status);
		for (int i = 0; i < size; i++) {
			group.at(i).update();
			dpoints[i] = group.at(i).getPoints();
		}
		if (timer > timeout && wait) {
			goup = true;
			moveWithScale(1);
			wait = false;
		}
		else
			timer++;
		if (!status[0] && goup) { //goup and move done
			for (int i = 0; i < size; i++) {
				group.at(i).roll();
			}
			goup = false;
			rolling = true;
			timer1 = 0;
			timeout1 = 30;
		}
		group.at(0).getStatus(status);
		if (!status[1] && rolling && timer1 > timeout1) { //rolling and roll done
			moveWithScale(2);
			godown = true;
			rolling = false;
		}
		else if (!status[1] && rolling) {
			timer1++;
		}
		group.at(0).getStatus(status);
		if (!status[0] && godown) { //godown and move done
			godown = false;
			pleasegetpoint = true;
		}
		delete[] status;
		if (pleasegetpoint) {
			pleasegetpoint = false;
			return dpoints;
		}
		dpoints[0] = -1;
		dpoints[1] = -1;
		return dpoints;
	}
	void move() {
		for (int i = 0; i < size; i++) {
			float mid = 0.5;
			if (dir == 0) {
				group.at(i).move(pos + vec3(0.0, 0.0, 0.5*(i - mid)));
			}
			else if (dir == 1) {
				group.at(i).move(pos + vec3(0.5*(i - mid), 0.0, 0.0));
			}
		}
	}
	void moveWithScale(int flag) {
		if (flag == 1) {
			for (int i = 0; i < size; i++) {
				float mid = 0.5;
				group.at(i).moveWithScale(vec3(0.0, 2.5, (i - mid)*2.0), flag);
			}
		}
		else if (flag == 2) {
			for (int i = 0; i < size; i++) {
				float mid = 0.5;
				if (dir == 0) {
					group.at(i).moveWithScale(pos + vec3(0.0, 0.0, 0.5*(i - mid)), flag);
				}
				else if (dir == 1) {
					group.at(i).moveWithScale(pos + vec3(0.5*(i - mid), 0.0, 0.0), flag);
				}
			}
		}
	}
	void setView(mat4 view) {
		this->view = view;
	}
	void setProj(GLuint um4p, mat4 proj) {
		this->proj = proj;
	}
	void finish() {
		timer = timeout+1;
	}
	void setPos(vec3 pos) {
		this->pos = pos;
		move();
	}
	vec3 getPos() {
		return pos;
	}
	void setDir(GLint dir) {
		this->dir = dir;
		setPos(pos);
	}
	void setTimeout(int t) {
		this->timeout = t;
	}
	void setPlayer(int p) {
		this->player = p;
	}
	int getPlayer() {
		return player;
	}
private:
	int size;
	vector<Dice> group;
	GLuint um4mv;
	GLuint um4p;
	mat4 view;
	mat4 proj;
	//roll
	bool rolling;
	bool moving;
	bool wait;
	bool goup;
	bool godown;
	int timer;
	int timeout;
	int timer1;
	int timeout1;
	int points;
	//player
	int player;
	bool pleasegetpoint;
	//pos
	vec3 pos;
	GLint dir;
};