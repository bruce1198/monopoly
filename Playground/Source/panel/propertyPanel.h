#pragma once

#ifndef PROPERTYPANEL
#define PROPERTYPANEL

#include "../window/texture.h"

class PropertyPanel {
public:
	bool isOpen;
	PropertyPanel() {
		isOpen = true;
	};
	PropertyPanel(GLuint program) {
		isOpen = true;

		string numberStr[10];
		for (int i = 0; i < 10; i++) {
			numberStr[i] = "panel/number/number_" + to_string(i) + ".png";
		}
		init(program, "panel/PanelPlayer1.png", "panel/PanelPlayer2.png", numberStr);
	};

	GLuint vaoPlayer1;
	GLuint vaoPlayer2;
	GLuint vaoNumberPlayer1[6];
	GLuint vaoNumberPlayer2[6];
	mat4 mvp;

	struct _Uniform {
		GLint mvp;
		GLint tex;
	} m_Uniform;

	void init(GLuint program, string imgStrPanelPlayer1, string imgStrPanelPlayer2, string imgStrNumber[]) {
		texturePanelPlayer1.init(imgStrPanelPlayer1.c_str());
		texturePanelPlayer2.init(imgStrPanelPlayer2.c_str());
		for (int i = 0; i < 10; i++) {
			textureNumber[i].init(imgStrNumber[i].c_str());
		}


		mvp = mat4(1.0f);

		//Uniform Location
		m_Uniform.mvp = glGetUniformLocation(program, "mvp");
		m_Uniform.tex = glGetUniformLocation(program, "tex");


		// vaoPlayer1
		glCreateVertexArrays(1, &vaoPlayer1);
		glBindVertexArray(vaoPlayer1);

		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		const float vertexData[] =
		{
			-1, 0.4, 0.0, 1.0,
			-1, 1, 0.0, 1.0,
			-0.5,  0.4, 0.0, 1.0,
			-0.5, 1, 0.0, 1.0,
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

		//NumbersVAO
		glGenVertexArrays(6, &vaoNumberPlayer1[0]);
		for (int i = 0; i < 6; i++) {
			glBindVertexArray(vaoNumberPlayer1[i]);

			GLuint vertexBuffer;
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			float center_x = -0.6 - i*0.05;
			float center_y = 0.6;
			float vertexDataNumbers[] =
			{
				center_x-0.025, center_y-0.05, 0.0, 1.0,
				center_x-0.025, center_y+0.05, 0.0, 1.0,
				center_x+0.025, center_y-0.05, 0.0, 1.0,
				center_x+0.025, center_y+0.05, 0.0, 1.0,
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexDataNumbers), vertexDataNumbers, GL_STATIC_DRAW);
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
		}


		// vaoPlayer2
		glCreateVertexArrays(1, &vaoPlayer2);
		glBindVertexArray(vaoPlayer2);
		
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		const float vertexData2[] =
		{
			0.5, 0.4, 0.0, 1.0,
			0.5, 1, 0.0, 1.0,
			1,  0.4, 0.0, 1.0,
			1, 1, 0.0, 1.0,
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData2), vertexData2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &texcoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoordData), texcoordData, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glGenVertexArrays(6, &vaoNumberPlayer2[0]);
		for (int i = 0; i < 6; i++) {
			glBindVertexArray(vaoNumberPlayer2[i]);

			GLuint vertexBuffer;
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			float center_x = 0.9 - i * 0.05;
			float center_y = 0.6;
			float vertexDataNumbers[] =
			{
				center_x - 0.025, center_y - 0.05, 0.0, 1.0,
				center_x - 0.025, center_y + 0.05, 0.0, 1.0,
				center_x + 0.025, center_y - 0.05, 0.0, 1.0,
				center_x + 0.025, center_y + 0.05, 0.0, 1.0,
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexDataNumbers), vertexDataNumbers, GL_STATIC_DRAW);
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
		}
	}

	void draw(GLuint program, int moneyPlayer1, int moneyPlayer2) {
		if (!isOpen)
			return;

		int moneyArrayPlayer1[6];
		int moneyArrayPlayer2[6];

		for (int i = 0; i < 6; i++) {
			moneyArrayPlayer1[i] = moneyPlayer1 % 10;
			moneyPlayer1 /= 10;
			moneyArrayPlayer2[i] = moneyPlayer2 % 10;
			moneyPlayer2 /= 10;
		}

		glUseProgram(program);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(vaoPlayer1);
			glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturePanelPlayer1.texture);
			glUniform1i(m_Uniform.tex, 0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glBindVertexArray(vaoPlayer2);
			glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturePanelPlayer2.texture);
			glUniform1i(m_Uniform.tex, 0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		//player 1 's money
		for (int i = 0; i < 6; i++) {
			glBindVertexArray(vaoNumberPlayer1[i]);
				glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureNumber[moneyArrayPlayer1[i]].texture);
				glUniform1i(m_Uniform.tex, 0);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}
		//player 2 's money
		for (int i = 0; i < 6; i++) {
			glBindVertexArray(vaoNumberPlayer2[i]);
				glUniformMatrix4fv(m_Uniform.mvp, 1, GL_FALSE, value_ptr(mvp));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureNumber[moneyArrayPlayer2[i]].texture);
				glUniform1i(m_Uniform.tex, 0);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}

		glDisable(GL_BLEND);
		glUseProgram(0);
	}

private:
	Texture2D texturePanelPlayer1;
	Texture2D texturePanelPlayer2;
	Texture2D textureNumber[10];
};

#endif // !PROPERTYPANEL

