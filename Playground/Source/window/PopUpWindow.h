#pragma once

#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include "texture.h"

class PopUpWindow {
public:
	bool wantOpen;
	float windowScale;

	PopUpWindow() { 
		wantOpen = true; 
		windowScale = 1.0f;
	};
	PopUpWindow(GLuint program) {
		wantOpen = false;
		windowScale = 0.0f;
		init(program);
	}

	GLuint vaoPanel;
	GLuint vaoButton_YES;
	GLuint vaoButton_NO;
	GLuint vaoPostionName;
	GLuint vaoPrice;
	mat4 mvp;
	GLint idx;
	// vec2 texOffset;

	struct _Uniform {
		GLint mvp;
		GLint tex;
		// GLint texOffset;
	} m_Uniform;
	
	void init(GLuint program) {
		type = 0;
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

		//postionName VAO
		glGenVertexArrays(1, &vaoPostionName);
		glBindVertexArray(vaoPostionName);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		const float vertexData4[] =
		{
			0.0, 0.3, 0.0, 1.0,
			0.0, 0.5, 0.0, 1.0,
			0.4, 0.3, 0.0, 1.0,
			0.4, 0.5, 0.0, 1.0,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData4), vertexData4, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &texcoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoordData), texcoordData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		//Price VAO
		glGenVertexArrays(1, &vaoPrice);
		glBindVertexArray(vaoPrice);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		const float vertexData5[] =
		{
			0.0, 0.1, 0.0, 1.0,
			0.0, 0.3, 0.0, 1.0,
			0.4, 0.1, 0.0, 1.0,
			0.4, 0.3, 0.0, 1.0,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData5), vertexData5, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &texcoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);

		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoordData), texcoordData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		texturePanel.init("window/woodPanel3.png");
		textureNormalBtn_YES.init("window/ButtonNormal_CYES.png");
		textureNormalBtn_NO.init("window/ButtonNormal_CNO.png");
		textureClickedBtn_YES.init("window/ButtonClicked_CYES.png");
		textureClickedBtn_NO.init("window/ButtonClicked_CNO.png");
		textureNormalBtn_A.init("window/ButtonNormal_A.png");
		textureNormalBtn_B.init("window/ButtonNormal_B.png");
		textureClickedBtn_A.init("window/ButtonClicked_A.png");
		textureClickedBtn_B.init("window/ButtonClicked_B.png");
		for (int i = 1; i <= 40; i++) {
			texturePositionName[i].init((string("window/positionName/positionName_") + to_string(i) + string(".png")).c_str()); //start from 1 to 40
		}
		for (int i = 1; i <= 40; i++) {
			texturePrice[i].init((string("window/price/price_") + to_string(i) + string(".png")).c_str()); //start from 1 to 40
		}
	}

	void drawPurchase(GLuint program, vec2 mousePos, vec2 currentWH) {
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
		glBindTexture(GL_TEXTURE_2D, texturePanel.texture);
		glUniform1i(m_Uniform.tex, 0);
		// glUniform2fv(m_Uniform.texOffset, 1, value_ptr(texOffset));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		glBindVertexArray(vaoButton_YES);

		glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
		glActiveTexture(GL_TEXTURE0);
		if (check(mousePos, currentWH) == 1) {
			if(type==0)
				glBindTexture(GL_TEXTURE_2D, textureClickedBtn_YES.texture);
			else
				glBindTexture(GL_TEXTURE_2D, textureClickedBtn_A.texture);
		}
		else {
			if(type==0)
				glBindTexture(GL_TEXTURE_2D, textureNormalBtn_YES.texture);
			else
				glBindTexture(GL_TEXTURE_2D, textureNormalBtn_A.texture);
		}
		glUniform1i(m_Uniform.tex, 0);
		// glUniform2fv(m_Uniform.texOffset, 1, value_ptr(texOffset));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		glBindVertexArray(vaoButton_NO);

		glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
		glActiveTexture(GL_TEXTURE0);
		if (check(mousePos, currentWH) == 2) {
			if (type == 0)
				glBindTexture(GL_TEXTURE_2D, textureClickedBtn_NO.texture);
			else
				glBindTexture(GL_TEXTURE_2D, textureClickedBtn_B.texture);
		}
		else {
			if (type == 0)
				glBindTexture(GL_TEXTURE_2D, textureNormalBtn_NO.texture);
			else
				glBindTexture(GL_TEXTURE_2D, textureNormalBtn_B.texture);
		}

		glUniform1i(m_Uniform.tex, 0);
		// glUniform2fv(m_Uniform.texOffset, 1, value_ptr(texOffset));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		glBindVertexArray(vaoPostionName);

		glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
		glActiveTexture(GL_TEXTURE0);
		//cout << idx;
		glBindTexture(GL_TEXTURE_2D, texturePositionName[idx].texture);
		glUniform1i(m_Uniform.tex, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);
		glBindVertexArray(vaoPrice);

		glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePrice[idx].texture);
		glUniform1i(m_Uniform.tex, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindVertexArray(0);


		glDisable(GL_BLEND);
		glUseProgram(0);
	}
	void setType(GLint type) {
		this->type = type;
	}
	int check(vec2 mousePos, vec2 currentWH) {
		vec2 mousePosD = (mousePos - currentWH / vec2(2, 2)) / currentWH;
		mousePosD *= 2;
		mousePosD.y = -mousePosD.y;

		if ((mousePosD.x < -0.05 && mousePosD.x > -0.35) && (mousePosD.y < -0.075 && mousePosD.y > -0.325)) {
			return 1;
		}
		else if ((mousePosD.x > 0.05 && mousePosD.x < 0.35) && (mousePosD.y < -0.075 && mousePosD.y > -0.325)) {
			return 2;
		}
		else {
			return 0;
		}
	}
private:
	GLint type;
	Texture2D texturePanel;
	Texture2D textureNormalBtn_YES;
	Texture2D textureNormalBtn_NO;
	Texture2D textureClickedBtn_YES;
	Texture2D textureClickedBtn_NO;
	Texture2D textureNormalBtn_A;
	Texture2D textureNormalBtn_B;
	Texture2D textureClickedBtn_A;
	Texture2D textureClickedBtn_B;
	Texture2D texturePositionName[41];
	Texture2D texturePrice[41];
}; 

#endif // ! POPUPWINDOW_H