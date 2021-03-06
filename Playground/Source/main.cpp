#include "chess/chess.h"
#include "dice/dice.h"
#include "card/card.h"
#include "map/map.h"
#include "house/house.h"
#include "logic/game.h"
#include "window/PopUpWindow.h"
#include "panel/propertyPanel.h"
#include "start/intro.h"
#include <thread>
//For GLUT to handle 
#define MENU_TIMER_START 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3

//timer
GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;
//screen
vec2 mousePos = vec2(0, 0);
vec2 currentWH = vec2(0, 0);
GLfloat screenWidth = 1600;
GLfloat screenHeight = 900;
//game object
Game game;
vector<Chess> chess;
DiceGroup dg;
CardGroup cg;
Map mymap;
Community community;
Camera camera;
PopUpWindow window;
PropertyPanel panel;
Button start_btn;
//shader
Shader chess_shader;
Shader dice_shader;
Shader card_shader;
Shader map_shader;
Shader house_shader;
Shader window_shader;
Shader depth_shader;
Shader shadow_shader;
Shader skybox_shader;
//
Skybox skybox;
//fbo
Fbo shadow_buffer;

// OpenGL initialization
void My_Init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND); 
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	srand((unsigned int)time(NULL));
	chess_shader = Shader("chess/chess.vertex", "chess/chess.fragment");
	dice_shader = Shader("dice/dice.vertex", "dice/dice.fragment");
	card_shader = Shader("card/card.vertex", "card/card.fragment");
	map_shader = Shader("map/map.vertex", "map/map.fragment");
	house_shader = Shader("house/house.vertex", "house/house.fragment");
	window_shader = Shader("window/window.vertex", "window/window.fragment");
	depth_shader = Shader("depth.vertex", "depth.fragment");
	shadow_shader = Shader("shadow.vertex", "shadow.fragment");
	skybox_shader = Shader("skybox/skybox.vertex", "skybox/skybox.fragment");
	//start
	Setting_Shader();
	panda.Load_Picture("start/panda.png");
	nthu.Load_Picture("start/nthu.png");
	nthu_door.Load_Picture("start/nthu_door.png");
	nthu_flag.Load_Picture("start/nthu_flag.png");
	monopoly_logo.Load_Picture("start/monopoly_logo.png");
	intro_buttom.Load_Picture("start/intro_buttom.png");
	start_btn.Load_Picture("start/start.jpg", "start/start2.jpg");
	//help.Load_Picture("help.jpg", "help2.jpg");
	//right.Load_Picture("right.jpg", "right2.jpg");
	//left.Load_Picture("left.jpg", "left2.jpg");
	frame_subject.Load_Frame_Subject();
	//
	game = Game(2);
	for (int i = 0; i < 2; i++) {
		chess.push_back(Chess(chess_shader.getProgram(), i));
	}
	dg = DiceGroup(2);
	cg = CardGroup(20);
	mymap = Map("map/map.png");
	skybox = Skybox(1);
	camera = Camera(vec3(0, 15, 0), vec3(0, 1, 0), 0, -30);
	window = PopUpWindow(window_shader.getProgram());
	panel = PropertyPanel(window_shader.getProgram());

	glGenFramebuffers(1, &shadow_buffer.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer.fbo);
	glGenRenderbuffers(1, &shadow_buffer.rbo);
	glGenTextures(1, &shadow_buffer.texture);
	glBindTexture(GL_TEXTURE_2D, shadow_buffer.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_buffer.texture, 0);

}
bool first = true;
void My_Display() {

	if (if_start) {
		if (first) {
			first = false;
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glEnable(GL_MULTISAMPLE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthFunc(GL_LEQUAL);
			camera.goTopView();
		}
		const float shadow_range = 30.0f;
		mat4 light_proj_matrix = ortho(-shadow_range, shadow_range, -shadow_range, shadow_range, 0.0f, 100.0f);
		mat4 light_view_matrix = lookAt(vec3(-4, 20, 10), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 light_vp_matrix = light_proj_matrix * light_view_matrix;
		mat4 scale_bias_matrix = translate(mat4(), vec3(0.5f, 0.5f, 0.5f)) * scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
		mat4 shadow_sbpv_matrix = scale_bias_matrix * light_vp_matrix;

		//-----------shadow fbo---------------------//
		glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer.fbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
		depth_shader.use();

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(4.0f, 4.0f);
		//draw card
		glUniformMatrix4fv(glGetUniformLocation(depth_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(light_proj_matrix));
		cg.setView(light_view_matrix);
		cg.draw(depth_shader.getProgram());
		//draw house
		community.setView(light_view_matrix);
		community.draw(depth_shader.getProgram());
		//draw dice
		dg.setView(light_view_matrix);
		dg.draw(depth_shader.getProgram());
		//draw chess
		for (Chess&c : chess) {
			c.setView(light_view_matrix);
			c.draw(depth_shader.getProgram(), true);
		}
		glDisable(GL_POLYGON_OFFSET_FILL);
		//---------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
		// Clear display buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mat4 view = camera.getViewMatrix();
		GLfloat viewportAspect = screenWidth / screenHeight;
		mat4 projection = perspective(camera.getZoom(), viewportAspect, 0.1f, 10000.0f);
		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skybox_shader.use();
		mat4 boxview = mat4(mat3(view));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.getProgram(), "view"), 1, GL_FALSE, value_ptr(boxview));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.getProgram(), "projection"), 1, GL_FALSE, value_ptr(projection));
		skybox.draw();
		glDepthMask(GL_TRUE);
		//draw chess
		for (Chess&c : chess) {
			c.setLightPos(vec3(-4, 20, 10));
			c.setProj(projection);
			c.setView(view);
			c.draw(chess_shader.getProgram(), false);
		}
		//draw dice
		dice_shader.use();
		glUniformMatrix4fv(glGetUniformLocation(dice_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(projection));
		dg.setView(view);
		dg.draw(dice_shader.getProgram());
		//draw card
		card_shader.use();
		glUniformMatrix4fv(glGetUniformLocation(card_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(projection));
		cg.setView(view);
		cg.draw(card_shader.getProgram());
		//draw map
		skybox.bind();
		map_shader.use();
		glUniformMatrix4fv(glGetUniformLocation(map_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(projection));
		mymap.setView(view);
		mymap.draw(map_shader.getProgram(), true);
		//draw house
		house_shader.use();
		glUniformMatrix4fv(glGetUniformLocation(house_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(projection));
		community.setView(view);
		community.draw(house_shader.getProgram());
		//draw shadow
		shadow_shader.use();
		mat4 shadow_matrix = shadow_sbpv_matrix * mymap.getModel();
		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, shadow_buffer.texture);
		glUniformMatrix4fv(glGetUniformLocation(shadow_shader.getProgram(), "shadow_matrix"), 1, GL_FALSE, value_ptr(shadow_matrix));
		glUniformMatrix4fv(glGetUniformLocation(shadow_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(projection));
		mymap.setView(view);
		mymap.draw(shadow_shader.getProgram(), false);
		//
		vector<Dice> group = dg.getGroup();
		for (Dice&d : group) {
			shadow_matrix = shadow_sbpv_matrix * d.getModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(shadow_shader.getProgram(), "shadow_matrix"), 1, GL_FALSE, value_ptr(shadow_matrix));
			glUniformMatrix4fv(glGetUniformLocation(shadow_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(shadow_shader.getProgram(), "um4mv"), 1, GL_FALSE, value_ptr(view*d.getModelMatrix()));
			d.draw(); 
		}
		//draw panel
		panel.setMoney(game.money[0], game.money[1]);
		panel.draw(window_shader.getProgram());
		//draw window
		window.drawPurchase(window_shader.getProgram(), mousePos, currentWH);
		//--
	}
	else {
		//
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_subject.fbo_origin);
		{
			glViewport(0, 0, intro.screen_width, intro.screen_height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			static const GLfloat purple[] = { 0.88, 0.63, 1.0, 1.0 };
			static const GLfloat one = 1.0f;
			glClearBufferfv(GL_COLOR, 0, purple);
			glClearBufferfv(GL_DEPTH, 0, &one);

			Draw_Panda();
			Draw_Nthu_Flag();
			Draw_Monopoly_Logo();
			Draw_Intro_Buttom();
			//glUniform1f(button_program.time, timer_cnt);
			start_btn.Draw_Button(0, 2.5, -0.12, 0.12, 0.24, 0.36);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_subject.fbo_darker);
		{
			glViewport(0, 0, intro.screen_width, intro.screen_height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			static const GLfloat purple[] = { 0.88, 0.63, 1.0, 1.0 };
			static const GLfloat red[] = { 1, 0,0, 1.0 };
			static const GLfloat one = 1.0f;
			glClearBufferfv(GL_COLOR, 0, red);
			glClearBufferfv(GL_DEPTH, 0, &one);
			frame_subject.Draw(frame_subject.texture_origin);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		frame_subject.Draw(frame_subject.texture_darker);
	}
	//
    glutSwapBuffers();
}
void My_Reshape(int width, int height) {
	currentWH = vec2(width, height);
	glViewport(0, 0, width, height);
	screenWidth = (GLfloat)width;
	screenHeight = (GLfloat)height;
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer.fbo);
	glBindTexture(GL_TEXTURE_2D, shadow_buffer.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_buffer.texture, 0);
	//
	intro.screen_width = width;
	intro.screen_height = height;
	float viewportAspect = (float)width / (float)height;
	intro.viewportRatio = viewportAspect;
	intro.projection = perspective(radians(80.0f), viewportAspect, 0.1f, 1000.0f);
	Frame_Subject_Configure(frame_subject.rbo_Sb, frame_subject.fbo_origin, frame_subject.texture_origin, width, height);
	Frame_Subject_Configure(frame_subject.rbo_Sb, frame_subject.fbo_darker, frame_subject.texture_darker, width, height);
}
int index;
void My_Timer(int val) {
	glutPostRedisplay();
	//update
	bool dis = cg.update();
	int* points = dg.update();
	bool done = camera.update();
	//event
	if (game.is_gameover) {
		window.gameover = true;
		window.wantOpen = true;
		window.windowScale = 1.0f;
	}
	if (camera.pass) {
		camera.pass = false;
		game.updatemoney = true;
	}
	if (game.goThrow) {
		game.goThrow = false;
		if (camera.getCurrentView() == 1) {
			dg.roll(0);
		}
		else {
			camera.goSideView();
			dg.roll(50);
		}
	}
	else if (game.goTop) {
		game.goTop = false;
		camera.goTopView();
		dg.setColor(game.doneidx);
	}
	else if (game.goSide) {
		game.goSide = false;
		camera.goSideView();
	}
	else if (game.popout) {
		game.popout = false;
		window.idx = index;
		if (index == 4)
			window.setType(1);
		else
			window.setType(0);
		window.wantOpen = true;
		window.windowScale = 0.0f;
	}
	else if (game.keetsu) {
		game.keetsu = false;
		House h(game.tsuidx, game.tsuheight, game.tsuowner);
		community.add(h);
	}
	else if (game.smove) {
		game.smove = false;
		chess[game.smoveidx].setPos(game.smovepos);
	}
	else if (game.getcard) {
		cg.getCard(game.cardtype, game.cardidx);
		game.getcard = false;
	}
	//
	int step = points[0]+points[1];
	if (step > 0) {
		game.getPoints = true;
		game.point = points;
		while (!game.response) {
			cout << "";
		}
		if (game.goWalk) {
			game.goWalk = false;
			index = game.from;
			camera.goPlayerView(index);
			camera.movePlayerView(index, step);
			index += step;
		}
		game.response = false;
	}
	if (done) {
		cout << "done" << endl;
		game.movedone = true;
		chess[game.doneidx].setPos(index);
	}
	if (dis) {
		cout << "card discard" << endl;
		game.getcarddone = true;
	}
	//
	glutTimerFunc(timer_speed, My_Timer, val);
}
void keyboard_down(unsigned char key, int x, int y) {
	if (key == 'w') {

	}
}
void keyboard_up(unsigned char key, int x, int y) {
	if (key == 't')
		camera.goTopView();
	else if (key == 's')
		camera.goSideView();
	else if (key == 'r') {
		game.canThrow = true;
	}
}

void My_SpecialKeys(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
		break;
	case GLUT_KEY_PAGE_UP:
		break;
	case GLUT_KEY_LEFT:
		//house.setPos(house.getPos()-1);
		break;
	case GLUT_KEY_RIGHT:
		//house.setPos(house.getPos() + 1);
		break;
	case GLUT_KEY_UP:
		//house.setPos(house.getPos() + 1);
		break;
	case GLUT_KEY_DOWN:
		//house.setPos(house.getPos() - 1);
		break;
	default:
		break;
	}
}

void My_Menu(int id)
{
	switch(id)
	{
	case MENU_TIMER_START:
		if(!timer_enabled) {
			timer_enabled = true;
			glutTimerFunc(timer_speed, My_Timer, 0);
		}
		break;
	case MENU_TIMER_STOP:
		timer_enabled = false;
		break;
	case MENU_EXIT:
		exit(0);
		break;
	default:
		break;
	}
}
bool firstMouse = true;
GLfloat lastX;
GLfloat lastY;
void My_Mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		start_btn.If_Click();
		switch (window.check(mousePos, currentWH)) {
		case 0:
			break;
		case 1:
			if (window.wantOpen&& window.windowScale) {
				if (!game.getanswer) {
					game.answer = 1;
					game.getanswer = true;
					window.wantOpen = false;
					window.windowScale = 1.0f;
				}
			}
			break;
		case 2:
			if (window.wantOpen&& window.windowScale) {
				if (!game.getanswer) {
					game.answer = 2;
					game.getanswer = true;
					window.wantOpen = false;
					window.windowScale = 1.0f;
				}
			}
			break;
		}
	}
	else if (state == GLUT_UP) {
		firstMouse = true;
	}
}
void My_PassiveMouse(int x, int y)
{
	mousePos = vec2(x, y);
	intro.mouseRatio = vec2(((x / intro.screen_width) * 2 - 1) * (intro.screen_width / (intro.screen_height * 16 / 9)), (y / intro.screen_height) * 2 - 1);
}
void My_Mouse_Move(int x, int y) {

	/*if (firstMouse)
	{
		lastX = (GLfloat)x;
		lastY = (GLfloat)y;
		firstMouse = false;
	}
	GLfloat xoffset = x - lastX;
	GLfloat yoffset = lastY - y;
	lastX = (GLfloat)x;
	lastY = (GLfloat)y;
	camera.mouseMovement(xoffset, yoffset, false);*/
}
void func() {
	game.Run();
}
int main(int argc, char *argv[])
{
#ifdef __APPLE__
    // Change working directory to source code path
    chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
#else
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1600, 900);
	glutCreateWindow("Monopoly"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif
	dumpInfo();
	My_Init();

	int menu_main = glutCreateMenu(My_Menu);
	int menu_timer = glutCreateMenu(My_Menu);

	glutSetMenu(menu_main);
	glutAddSubMenu("Timer", menu_timer);
	glutAddMenuEntry("Exit", MENU_EXIT);

	glutSetMenu(menu_timer);
	glutAddMenuEntry("Start", MENU_TIMER_START);
	glutAddMenuEntry("Stop", MENU_TIMER_STOP);

	glutSetMenu(menu_main);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Register GLUT callback functions.
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutPassiveMotionFunc(My_PassiveMouse);
	glutMotionFunc(My_Mouse_Move);
	glutKeyboardUpFunc(keyboard_up);
	glutKeyboardFunc(keyboard_down);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(timer_speed, My_Timer, 0);
	glutMouseFunc(My_Mouse);

	thread mThread(func);
	glutMainLoop();

	return 0;
}