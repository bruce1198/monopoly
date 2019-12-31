#include "dice/dice.h"
#include "map/map.h"
#include "logic/game.h"
#include "window/PopUpWindow.h"
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
DiceGroup dg;
Map mymap;
Camera camera;
PopUpWindow window;
//shader
Shader dice_shader;
Shader map_shader;
Shader window_shader;

// OpenGL initialization
void My_Init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	srand((unsigned int)time(NULL));
	dice_shader = Shader("dice/dice.vertex", "dice/dice.fragment");
	map_shader = Shader("map/map.vertex", "map/map.fragment");
	window_shader = Shader("window/window.vertex", "window/window.fragment");
	game = Game(2);
	dg = DiceGroup(2);
	mymap = Map("map/Plane.obj", "map/bigger2.png");
	camera = Camera(vec3(0, 15, 0), vec3(0, 1, 0), 0, -30);
	window = PopUpWindow(window_shader.getProgram());
	camera.goTopView();
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	// Clear display buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	mat4 view = camera.getViewMatrix();
	GLfloat viewportAspect = screenWidth / screenHeight;;
	mat4 projection = perspective(camera.getZoom(), viewportAspect, 0.1f, 100.0f);
	//draw dice
	dice_shader.use();
	glUniformMatrix4fv(glGetUniformLocation(dice_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(projection));
	dg.setView(view);
	dg.draw(dice_shader.getProgram());
	//draw map
	map_shader.use();
	glUniformMatrix4fv(glGetUniformLocation(map_shader.getProgram(), "um4p"), 1, GL_FALSE, value_ptr(projection));
	mymap.setView(view);
	mymap.draw(map_shader.getProgram());
	//draw window
	window.draw(window_shader.getProgram(), mousePos, currentWH);
    glutSwapBuffers();
}
void My_Reshape(int width, int height) {
	currentWH = vec2(width, height);
	glViewport(0, 0, width, height);
	screenWidth = (GLfloat)width;
	screenHeight = (GLfloat)height;
}
void My_Timer(int val) {
	glutPostRedisplay();
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
	else if (game.goSide) {
		game.goSide = false;
		camera.goTopView();
	}
	else if (game.popout) {
		game.popout = false;
		window.wantOpen = true;
		window.windowScale = 0.0f;
	}
	int* points = dg.update();
	int step = points[0]+points[1];
	if (step > 0) {
		game.getPoints = true;
		game.point = points;
		while (!game.response) {
			cout << "";
		}
		if (game.goWalk) {
			game.goWalk = false;
			int index = game.from;
			camera.goPlayerView(index);
			camera.movePlayerView(index, step);
			index += step;
		}
		game.response = false;
	}
	if (camera.update()) {
		game.movedone = true;
	};
	glutTimerFunc(timer_speed, My_Timer, val);
}
void keyboard_down(unsigned char key, int x, int y) {
	/*if (key == 'w')
		camera.direction[0] = true;
	else if (key == 's')
		camera.direction[1] = true;
	else if (key == 'a')
		camera.direction[2] = true;
	else if (key == 'd')
		camera.direction[3] = true;
	else if (key == 'z')
		camera.direction[4] = true;
	else if (key == 'x')
		camera.direction[5] = true;*/
}
void keyboard_up(unsigned char key, int x, int y) {
	/*if (key == 'w')
		camera.direction[0] = false;
	else if (key == 's')
		camera.direction[1] = false;
	else if (key == 'a')
		camera.direction[2] = false;
	else if (key == 'd')
		camera.direction[3] = false;
	else if (key == 'z')
		camera.direction[4] = false;
	else if (key == 'x')
		camera.direction[5] = false;*/
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
		break;
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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