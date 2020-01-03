#pragma once

#ifndef CHESS_H
#define CHESS_H
#include "../util.h"

class Chess {
public:
	int position;
	int index_count;
	int index;
	Chess (){
		position = 0;
		index_count = 0;
	};
	Chess(GLuint program, int index) {
		index_count = 0;
		init(program, index);
	};

	GLuint vao;
	mat4 m_matrix;
	mat4 v_matrix;
	mat4 p_matrix;
	vec3 diffuse_albedo;
	vec3 lightPos;

	struct _Uniform {
		GLint mv_matrix;
		GLint p_matrix;

		GLint diffuse_albedo;
		GLint lightPos;
	}m_Uniform;
	
	void setView(mat4 view) {
		this->v_matrix = view;
	}
	void setProj(mat4 proj) {
		this->p_matrix = proj;
	}
	void setLightPos(vec3 light) {
		lightPos = light;
	}
	void setPos(int position) {
		this->position = position;
		vec3 trans;
		position %= 40;
		switch (position) {
		case 0:
			trans = vec3(4+index*0.6, 0, -4);
			break;
		case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
			trans = vec3(4 + index * 0.6, 0, -3.8 + position * 8.0f / 10.5);
			break;
		case 10:
			trans = vec3(4 + index * 0.6, 0, 4);
			break;
		case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:
			trans = vec3(3.8 - (position - 10) * 8.0f / 10.5, 0, 4 + index * 0.6);
			break;
		case 20:
			trans = vec3(-4, 0, 4 + index * 0.6);
			break;
		case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:
			trans = vec3(-4 - index * 0.6, 0, 3.8 - (position - 20) * 8.0f / 10.5);
			break;
		case 30:
			trans = vec3(-4 - index * 0.6, 0, -4);
			break;
		case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:
			trans = vec3(-3.8 + (position - 30) * 8.0f / 10.5, 0, -4 - index * 0.6);
			break;
		default:
			break;
		}
		m_matrix = translate(mat4(), trans);
		m_matrix = scale(m_matrix, vec3(0.2f, 0.2f, 0.2f));
	}
	GLint getPos() {
		return position;
	}
	
	void init(GLuint program, int idx) {
		this->index = idx;
		setPos(0);
		if (idx == 0) {
			diffuse_albedo = vec3(0.35f, 0.1f, 1.0f);
		}
		else if (idx == 1) {
			diffuse_albedo = vec3(1.0f, 0.1f, 0.35f);
		}

		m_Uniform.mv_matrix = glGetUniformLocation(program, "mv_matrix");
		m_Uniform.p_matrix = glGetUniformLocation(program, "p_matrix");
		m_Uniform.diffuse_albedo = glGetUniformLocation(program, "diffuse_albedo");
		m_Uniform.lightPos = glGetUniformLocation(program, "lightPos");

		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;
		string err;
		tinyobj::LoadObj(shapes, materials, err, "chess/chess.obj");

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint position_buffer;
		GLuint normal_buffer;
		GLuint index_buffer;

		glGenBuffers(1, &position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
		glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &normal_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
		glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.normals.size() * sizeof(float), shapes[0].mesh.normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size() * sizeof(unsigned int), shapes[0].mesh.indices.data(), GL_STATIC_DRAW);
		index_count = shapes[0].mesh.indices.size();
	}

	void draw(GLuint program, bool shadow) {
		glUseProgram(program);
			
			glBindVertexArray(vao);
			if (!shadow) {
				glUniformMatrix4fv(m_Uniform.mv_matrix, 1, GL_FALSE, &(v_matrix * m_matrix)[0][0]);
				glUniformMatrix4fv(m_Uniform.p_matrix, 1, GL_FALSE, &p_matrix[0][0]);
				glUniform3fv(m_Uniform.diffuse_albedo, 1, &diffuse_albedo[0]);
				glUniform3fv(m_Uniform.lightPos, 1, &lightPos[0]);
			}
			else 
				glUniformMatrix4fv(glGetUniformLocation(program, "um4mv"), 1, GL_FALSE, value_ptr(v_matrix * m_matrix));

			glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		glUseProgram(0);
	}

private:

};

#endif