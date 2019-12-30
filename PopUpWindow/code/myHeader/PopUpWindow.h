#pragma once

#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include "../Include.h"

using namespace glm;
using namespace std;

class PopUpWindow {
public:
	bool wantOpen;
	float windowScale;

	PopUpWindow() { 
		wantOpen = true; 
		windowScale = 1.0f;
	};

	GLuint vaoPanel;
	GLuint vaoButton_YES;
	GLuint vaoButton_NO;
	mat4 mvp;
	// vec2 texOffset;

	struct _Uniform {
		GLint mvp;
		GLint tex;
		// GLint texOffset;
	} m_Uniform;
	
	void init(GLuint program) {
		mvp = mat4(1.0f);
		// texOffset = vec2(-400, -159);

		//Uniform Location
		m_Uniform.mvp = glGetUniformLocation(program, "mvp");
		m_Uniform.tex = glGetUniformLocation(program, "tex");
		// m_Uniform.texOffset = glGetUniformLocation(program, "texOffset");

		glCreateVertexArrays(1, &vaoPanel);
		glBindVertexArray(vaoPanel);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		const float vertexData[] =
		{
			-0.5, -0.65, 0.0, 1.0,
			-0.5, 0.65, 0.0, 1.0,
			0.5,  -0.65, 0.0, 1.0,
			0.5, 0.65, 0.0, 1.0,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		GLuint texcoordBuffer;
		glGenBuffers(1, &texcoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
		const float texcoordData[] =
		{
			0.0, 0.0,
			0.0, 1.0,
			1.0, 0.0,
			1.0, 1.0
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoordData), texcoordData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		//buttonVAO_YES
		glGenVertexArrays(1, &vaoButton_YES);
		glBindVertexArray(vaoButton_YES);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		const float vertexData2[] =
		{
			-0.35, -0.325, 0.0, 1.0,
			-0.35, -0.075, 0.0, 1.0,
			-0.05, -0.325, 0.0, 1.0,
			-0.05, -0.075, 0.0, 1.0,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData2), vertexData2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &texcoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoordData), texcoordData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		//buttonVAO_NO
		glGenVertexArrays(1, &vaoButton_NO);
		glBindVertexArray(vaoButton_NO);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		const float vertexData3[] =
		{
			0.05, -0.325, 0.0, 1.0,
			0.05, -0.075, 0.0, 1.0,
			0.35, -0.325, 0.0, 1.0,
			0.35, -0.075, 0.0, 1.0,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData3), vertexData3, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &texcoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoordData), texcoordData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

	}

	void draw(GLuint program, GLuint texturePanel, GLuint textureNormalBtn_YES, GLuint textureNormalBtn_NO, 
		GLuint textureClickedBtn_YES, GLuint textureClickedBtn_NO, vec2 mousePos, vec2 currentWH) {
		if (wantOpen) {
			if (windowScale < 1.0f) {
				windowScale += 0.1f;
				mvp = scale(mat4(), vec3(windowScale));
			}
		}
		else {
			if (windowScale > 0.0f) {
				windowScale -= 0.1f;
				mvp = scale(mat4(), vec3(windowScale));
			}
		}
		if (windowScale == 0.0f) {
			return;
		}

		glUseProgram(program);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(vaoPanel);

			glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturePanel);
			glUniform1i(m_Uniform.tex, 0);
			// glUniform2fv(m_Uniform.texOffset, 1, value_ptr(texOffset));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glBindVertexArray(0);	
		glBindVertexArray(vaoButton_YES);

			glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
			glActiveTexture(GL_TEXTURE0);
			if (check(mousePos, currentWH) == 1) {
				glBindTexture(GL_TEXTURE_2D, textureClickedBtn_YES);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, textureNormalBtn_YES);
			}
			glUniform1i(m_Uniform.tex, 0);
			// glUniform2fv(m_Uniform.texOffset, 1, value_ptr(texOffset));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		glBindVertexArray(vaoButton_NO);

			glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
			glActiveTexture(GL_TEXTURE0);
			if (check(mousePos, currentWH) == 2) {
				glBindTexture(GL_TEXTURE_2D, textureClickedBtn_NO);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, textureNormalBtn_NO);
			}
			
			glUniform1i(m_Uniform.tex, 0);
			// glUniform2fv(m_Uniform.texOffset, 1, value_ptr(texOffset));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);

		glDisable(GL_BLEND);
		glUseProgram(0);
	}

	int check(vec2 mousePos, vec2 currentWH) {
		vec2 mousePosD = (mousePos - currentWH / vec2(2, 2)) / currentWH;
		mousePosD *= 2;
		mousePosD.y = -mousePosD.y;

		if ((mousePosD.x < -0.05 && mousePosD.x > -0.35) && (mousePosD.y < -0.075 && mousePosD.y > -0.325)) {
			return 1;
		}
		else if ((mousePosD.x > 0.05 && mousePosD.x < 0.35) && (mousePosD.y < -0.075 && mousePosD.y > -0.325)){
			return 2;
		}
		else {
			return 0;
		}
		
	}
private:

}; 

#endif // ! POPUPWINDOW_H