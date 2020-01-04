#pragma once
#ifndef _INTRO_H_
#define _INTRO_H_

#include "../util.h"


static const float window_positions[] = {

	 1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 1.0f, 1.0f
};

float planeVertices[] = {
	// positions            // normals         // texcoords
	 1.0f, -0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	-1.0f, -0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-1.0f, -0.0f, -1.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

	 1.0f, -0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	-1.0f, -0.0f, -1.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
	 1.0f, -0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
};
int if_start = 0;

class MyProgram {
public:
	GLuint program;
	mat4 mvp;
	GLint GPU_model;
	GLint GPU_view;
	GLint GPU_projection;
	GLint GPU_mvp;
	GLint time;
};
MyProgram picture_program, button_program;

class INTRO
{
public:
	mat4 view;
	mat4 projection;
	mat4 model;
	//GLuint program_button;
	vec3 cameraPos = vec3(0, 10, 0);
	vec3 cameraTarget = vec3{ 0,0,0 };
	vec3 up = vec3(0.0f, 0.0f, -1.0f);
	
	GLfloat viewportRatio;
	GLfloat screen_width;
	GLfloat screen_height;
	vec2 mouseRatio = vec2(0, 0);
	vec2 mouse;
	
	
};
INTRO intro;



class Picture
{
public:
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint texture;
	mat4 model;


	void Load_Picture(const char* png)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);

		TextureData tdata;
		float borderColor[] = { 0.0f, 0.0f, 1.0f,1.0f };
		tdata = load_png(png, true);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindTexture(GL_TEXTURE_2D, 0);
	};

	void Draw()
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	};
};

Picture panda, nthu_door, monopoly_logo, nthu, nthu_flag, intro_buttom;

class Button
{
public:
	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint texture;
	GLuint texture2;
	GLuint click;
	bool ontop;
	float x1, x2, y1, y2;
	mat4 model;
	void Load_Picture(const char* png, const char* png2)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);

		TextureData tdata;
		tdata = load_png(png, true);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		tdata = load_png(png2, true);
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	};

	void Draw()
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	};
	void Draw_Button(float x, float z, float x1_2D, float x2_2D, float y1_2D, float y2_2D)
	{
		glUseProgram(picture_program.program);

		intro.view = lookAt(intro.cameraPos, intro.cameraTarget, intro.up);
		glUniformMatrix4fv(picture_program.GPU_view, 1, GL_FALSE, value_ptr(intro.view));
		glUniformMatrix4fv(picture_program.GPU_projection, 1, GL_FALSE, value_ptr(intro.projection));
		{
			double degree = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		
			if (click == 1)
				glBindTexture(GL_TEXTURE_2D, texture);
			else 
				glBindTexture(GL_TEXTURE_2D, texture2);
		
			model = translate(mat4(), vec3(x, 0, z)) * scale(mat4(), vec3(2.0f, 1.0f, 0.5f)) ;
			glUniformMatrix4fv(picture_program.GPU_model, 1, GL_FALSE, value_ptr(model));
			Draw();
		}
		x1 = x1_2D;
		x2 = x2_2D;
		y1 = y1_2D;
		y2 = y2_2D;
		If_Hover();
			
	}
	void If_Click() {
		if (intro.mouseRatio.y > y1 && intro.mouseRatio.y < y2  && intro.mouseRatio.x > x1 && intro.mouseRatio.x < x2) {
			click = 0;
			if_start = 1;
		}
		/*else {
			click = 1;
		}*/
	}
	void If_Hover() {
		//printf("mouseRatio.x = %f\n", mouseRatio.x);
		//printf("mouseRatio.y = %f\n", mouseRatio.y);

		if (intro.mouseRatio.y > y1 && intro.mouseRatio.y < y2  && intro.mouseRatio.x > x1 && intro.mouseRatio.x < x2) {
			click = 1;
			ontop = true;
		}
		else {
			click = 0;
			ontop = false;
		}
		
	}



};

class Frame_Subject
{
public:
	GLuint program,program_darker;
	GLuint fbo_origin, fbo_darker, fbo_Snoobj;
	GLuint rbo_Sb, rbo_Sobj; // rbo_Snoobj = rbo_Sobj
	GLuint texture_origin, texture_darker, texture_Snoobj;
	GLuint vao;
	GLuint vbo;
	void Load_Frame_Subject()
	{

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(window_positions), window_positions, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (const GLvoid*)(sizeof(GL_FLOAT) * 2));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glGenFramebuffers(1, &fbo_origin);
		glGenFramebuffers(1, &fbo_darker);
		glGenFramebuffers(1, &fbo_Snoobj);


	};
	void Draw(GLuint texture)
	{
		glUseProgram(program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	void Draw_Darker()//GLuint origin, GLuint darker
	{
		glUseProgram(program_darker);
		
		glActiveTexture(GL_TEXTURE0);
		//glUniform1i(glGetUniformLocation(program, "origin"), 0);
		glBindTexture(GL_TEXTURE_2D, texture_origin);

		//glActiveTexture(GL_TEXTURE1);
		//glUniform1i(glGetUniformLocation(program_darker, "darker"), 1);
		//glBindTexture(GL_TEXTURE_2D, texture_darker);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);
	}
};
Frame_Subject frame_subject;

class Page
{
public:
	int page;
	

};
Page page;

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

void freeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}

int loadShader(const char* vsPath, const char* fsPath)
{
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource = loadShaderSource(vsPath);
	char** fsSource = loadShaderSource(fsPath);
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	freeShaderSource(vsSource);
	freeShaderSource(fsSource);
	glCompileShader(vs);
	glCompileShader(fs);
	shaderLog(vs);
	shaderLog(fs);

	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	return shaderProgram;
}

void Setting_Shader() { // init
	picture_program.program = loadShader("start/vertex.vs.glsl", "start/fragment_image.fs.glsl");
	frame_subject.program = loadShader("start/vertex_frame_subject.vs.glsl", "start/fragment_frame_subject.fs.glsl");
	frame_subject.program_darker = loadShader("start/vertex_frame_subject.vs.glsl", "start/fragment_frame_subject_black.fs.glsl");
	button_program.program = loadShader("start/vertex_button.vs.glsl", "start/fragment_button.fs.glsl");

	picture_program.GPU_projection = glGetUniformLocation(picture_program.program, "position");
	picture_program.GPU_view = glGetUniformLocation(picture_program.program, "view");
	picture_program.GPU_model = glGetUniformLocation(picture_program.program, "model");
	picture_program.time = glGetUniformLocation(picture_program.program, "time");
	//intro.GPU_mvp = glGetUniformLocation(intro.program_picture, "mvp");

	button_program.GPU_projection = glGetUniformLocation(button_program.program, "position");
	button_program.GPU_view = glGetUniformLocation(button_program.program, "view");
	button_program.GPU_model = glGetUniformLocation(button_program.program, "model");
	button_program.time = glGetUniformLocation(button_program.program, "time");
	//intro.GPU_mvp2 = glGetUniformLocation(intro.program_button, "mvp");
}


void Draw_Back()
{

	glUseProgram(picture_program.program);
	{
		intro.view = lookAt(intro.cameraPos, intro.cameraTarget, intro.up);
		glUniformMatrix4fv(picture_program.GPU_view, 1, GL_FALSE, value_ptr(intro.view));
		glUniformMatrix4fv(picture_program.GPU_projection, 1, GL_FALSE, value_ptr(intro.projection));

		double degree = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		glBindTexture(GL_TEXTURE_2D, intro_buttom.texture);
		mat4 rotation_matrix = rotate(mat4(), 0.3f, vec3(0.0, 1.0, 0.0));
		intro_buttom.model = translate(mat4(), vec3(0, 0, 5.5)) * scale(mat4(), vec3(16.0f, 1.0f, 3.0f));
		glUniformMatrix4fv(picture_program.GPU_model, 1, GL_FALSE, value_ptr(intro_buttom.model));
		intro_buttom.Draw();

	}

}

void Draw_Panda()
{

	glUseProgram(picture_program.program);
	{
		intro.view = lookAt(intro.cameraPos, intro.cameraTarget, intro.up);
		glUniformMatrix4fv(picture_program.GPU_view, 1, GL_FALSE, value_ptr(intro.view));
		glUniformMatrix4fv(picture_program.GPU_projection, 1, GL_FALSE, value_ptr(intro.projection));

		double degree = glutGet(GLUT_ELAPSED_TIME) / 500.0;
		glBindTexture(GL_TEXTURE_2D, panda.texture);
		mat4 rotation_matrix = rotate(mat4(), 0.5f, vec3(0.0,1.0,0.0));
		panda.model = translate(mat4(), vec3(0, 0, 0.05*sin(degree)-5.5 ))*scale(mat4(), vec3(4.0f, 1.0f, 4.0f));
		glUniformMatrix4fv(picture_program.GPU_model, 1, GL_FALSE, value_ptr(panda.model));
		panda.Draw();

	}
}

void Draw_Nthu()
{

	glUseProgram(picture_program.program);
	{
		intro.view = lookAt(intro.cameraPos, intro.cameraTarget, intro.up);
		glUniformMatrix4fv(picture_program.GPU_view, 1, GL_FALSE, value_ptr(intro.view));
		glUniformMatrix4fv(picture_program.GPU_projection, 1, GL_FALSE, value_ptr(intro.projection));

		double degree = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		glBindTexture(GL_TEXTURE_2D, nthu.texture);
		mat4 rotation_matrix = rotate(mat4(), 0.3f, vec3(0.0, 1.0, 0.0));
		nthu.model =  translate(mat4(), vec3(-3,0,-1))*rotation_matrix*scale(mat4(), vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(picture_program.GPU_model, 1, GL_FALSE, value_ptr(nthu.model));
		nthu.Draw();

	}
}

void Draw_Monopoly_Logo()
{

	glUseProgram(picture_program.program);
	{
		intro.view = lookAt(intro.cameraPos, intro.cameraTarget, intro.up);
		glUniformMatrix4fv(picture_program.GPU_view, 1, GL_FALSE, value_ptr(intro.view));
		glUniformMatrix4fv(picture_program.GPU_projection, 1, GL_FALSE, value_ptr(intro.projection));

		double degree = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		glBindTexture(GL_TEXTURE_2D, monopoly_logo.texture);

		mat4 rotation_matrix = rotate(mat4(), 0.3f, vec3(0.0, 1.0, 0.0));
		monopoly_logo.model =  translate(mat4(), vec3(0, 0, -0.5))*scale(mat4(), vec3(11.0f, 1.0f, 2.0f)) ;
		glUniformMatrix4fv(picture_program.GPU_model, 1, GL_FALSE, value_ptr(monopoly_logo.model));
		//glUniformMatrix4fv(intro.GPU_mvp, 1, GL_FALSE, value_ptr(monopoly_logo.model));
		monopoly_logo.Draw();

	}
}

void Draw_Intro_Buttom()
{

	glUseProgram(picture_program.program);
	{
		intro.view = lookAt(intro.cameraPos, intro.cameraTarget, intro.up);
		glUniformMatrix4fv(picture_program.GPU_view, 1, GL_FALSE, value_ptr(intro.view));
		glUniformMatrix4fv(picture_program.GPU_projection, 1, GL_FALSE, value_ptr(intro.projection));

		double degree = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		glBindTexture(GL_TEXTURE_2D, intro_buttom.texture);
		mat4 rotation_matrix = rotate(mat4(), 0.3f, vec3(0.0, 1.0, 0.0));
		intro_buttom.model =  translate(mat4(), vec3(0, 0, 5.5)) * scale(mat4(), vec3(16.0f, 1.0f, 3.0f)) ;
		glUniformMatrix4fv(picture_program.GPU_model, 1, GL_FALSE, value_ptr(intro_buttom.model));
		intro_buttom.Draw();

	}

}

void Draw_Nthu_Flag()
{

	glUseProgram(picture_program.program);
	{

		intro.view = lookAt(intro.cameraPos, intro.cameraTarget, intro.up);
		glUniformMatrix4fv(picture_program.GPU_view, 1, GL_FALSE, value_ptr(intro.view));
		glUniformMatrix4fv(picture_program.GPU_projection, 1, GL_FALSE, value_ptr(intro.projection));
		double degree = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		glBindTexture(GL_TEXTURE_2D, nthu_flag.texture);
		mat4 rotation_matrix = rotate(mat4(), -0.3f, vec3(0.0, 1.0, 0.0));
		nthu_flag.model = panda.model  * translate(mat4(), vec3(-1.0, 0, 0.0))*rotation_matrix;
		glUniformMatrix4fv(picture_program.GPU_model, 1, GL_FALSE, value_ptr(nthu_flag.model));
		nthu.Draw();

	}
}

void Frame_Subject_Configure(GLuint rbo, GLuint fbo, GLuint &texture, int width, int height)
{
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &texture);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	/*{
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");
		else
			printf("YEAH! complete!\n");
	}*/
}


void Intro_My_Reshape(int width, int height)
{

};

#endif