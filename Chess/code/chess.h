#pragma once

#ifndef CHESS_H
#define CHESS_H
#include "../Include.h"

using namespace std;
using namespace glm;

class Chess {
public:
	int position;
	int index_count;
	Chess (){
		position = 1; //TODO maybe adjust to 0?? '0-39' or '1-40'
		index_count = 0;
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
	
	
	void init(GLuint program) {
		m_matrix = translate(mat4(1.0f), vec3(1.0f, 0.0f, 0.0f)); //TODO adjust with variable "position"
		v_matrix = lookAt(vec3(5.0f, 5.0f, 5.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f)); //TODO adjust to camera's view
		p_matrix = perspective(radians(80.0f), 1600.0f / 900.0f, 0.1f, 10000.0f); //TODO adjust to camera's projection
		diffuse_albedo = vec3(0.35f, 0.1f, 1.0f); //TODO adjust different color with different players
		lightPos = vec3(50.0f, 50.0f, -50.0f); // TODO adjust to your light position

		m_Uniform.mv_matrix = glGetUniformLocation(program, "mv_matrix");
		m_Uniform.p_matrix = glGetUniformLocation(program, "p_matrix");
		m_Uniform.diffuse_albedo = glGetUniformLocation(program, "diffuse_albedo");
		m_Uniform.lightPos = glGetUniformLocation(program, "lightPos");

		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;
		string err;
		tinyobj::LoadObj(shapes, materials, err, "chess.obj");

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

	void draw(GLuint program) {
		glUseProgram(program);
			
			glBindVertexArray(vao);
			glUniformMatrix4fv(m_Uniform.mv_matrix, 1, GL_FALSE, &(v_matrix * m_matrix)[0][0]);
			glUniformMatrix4fv(m_Uniform.p_matrix, 1, GL_FALSE, &p_matrix[0][0]);
			glUniform3fv(m_Uniform.diffuse_albedo, 1, &diffuse_albedo[0]);
			glUniform3fv(m_Uniform.lightPos, 1, &lightPos[0]);

			glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		glUseProgram(0);
	}

private:

};

#endif