#include "map.h"

//For GLUT to handle 
#define MENU_TIMER_START 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3

//GLUT timer variable
GLubyte timer_cnt = 0;
bool timer_enabled = true;
unsigned int timer_speed = 16;

mat4 view;					// V of MVP, viewing matrix
mat4 projection;			// P of MVP, projection matrix

GLint um4p;	

GLuint program;			// shader program id

Map mymap;
Camera camera;
GLfloat screenWidth = 1600;
GLfloat screenHeight = 900;

// OpenGL initialization
void My_Init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);

	// Create Shader Program
    program = glCreateProgram();

	// Create customize shader by tell openGL specify shader type 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader file
	char** vertexShaderSource = loadShaderSource("map.vertex");
	char** fragmentShaderSource = loadShaderSource("map.fragment");

	// Assign content of these shader files to those shaders we created before
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

	// Free the shader file string(won't be used any more)
	freeShaderSource(vertexShaderSource);
	freeShaderSource(fragmentShaderSource);

	// Compile these shaders
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

	// Logging
	shaderLog(vertexShader);
    shaderLog(fragmentShader);

	// Assign the program we created before with these shaders
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

	// Get the id of inner variable 'um4p' and 'um4mv' in shader programs
	um4p = glGetUniformLocation(program, "um4p");

	// Tell OpenGL to use this shader program now
    glUseProgram(program);
   
	mymap = Map("Plane.obj", "bigger.png");
	camera = Camera(vec3(0.0, 15, 0), vec3(0,1 ,0), 0, -30);
	srand(time(NULL));
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	// Clear display buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	//glClearColor(1.0, 0.0, 0.0, 1.0);

	view = camera.getViewMatrix();
	GLfloat viewportAspect = screenWidth / screenHeight;
	projection = perspective(camera.getZoom(), viewportAspect, 0.1f, 1000.0f);
	glUniformMatrix4fv(um4p, 1, GL_FALSE, value_ptr(projection));
	mymap.setView(view);
	mymap.draw(program);
  
	// Change current display buffer to another one (refresh frame) 
    glutSwapBuffers();
}

// Setting up viewing matrix
void My_Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	screenWidth = width;
	screenHeight = height;
}

void My_Timer(int val)
{
	timer_cnt++;
	glutPostRedisplay();
	if(timer_enabled) {
		camera.update();
		glutTimerFunc(timer_speed, My_Timer, val);
	}
}
void keyboard_click(unsigned char key, int x, int y) {
	if (key == 't')
		camera.goTopView();
}
void keyboard_down(unsigned char key, int x, int y) {
	if (key == 'w')
		camera.direction[0] = true;
	if (key == 's')
		camera.direction[1] = true;
	if (key == 'a')
		camera.direction[2] = true;
	if (key == 'd')
		camera.direction[3] = true;
	if (key == 'z')
		camera.direction[4] = true;
	if (key == 'x')
		camera.direction[5] = true;
}
int index = 0;
void keyboard_up(unsigned char key, int x, int y) {
	if (key == 'w')
		camera.direction[0] = false;
	if (key == 's')
		camera.direction[1] = false;
	if (key == 'a')
		camera.direction[2] = false;
	if (key == 'd')
		camera.direction[3] = false;
	if (key == 'z')
		camera.direction[4] = false;
	if (key == 'x')
		camera.direction[5] = false;
	if (key == 't')
		camera.goTopView();
	if (key == 'y')
		camera.goSideView();
	if (key == 'r') {
		int step = (float)rand() / RAND_MAX * 11 + 1;
		cout << step << endl;
		camera.goPlayerView(index);
		camera.movePlayerView(index, step);
		index += step;
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
		
	}
	else if (state == GLUT_UP) {
		firstMouse = true;
	}
}
void My_Mouse_Move(int x, int y) {

	if (firstMouse)
	{
		lastX = (GLfloat)x;
		lastY = (GLfloat)y;
		firstMouse = false;
	}
	GLfloat xoffset = x - lastX;
	GLfloat yoffset = lastY - y;
	lastX = (GLfloat)x;
	lastY = (GLfloat)y;
	camera.mouseMovement(xoffset, yoffset, false);
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
	glutCreateWindow("Map"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
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
	glutMotionFunc(My_Mouse_Move);
	glutKeyboardUpFunc(keyboard_up);
	glutKeyboardFunc(keyboard_down);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(timer_speed, My_Timer, 0);
	// Todo
	// Practice 1 : Register new GLUT event listner here
	// ex. glutXXXXX(my_Func);
	// Remind : you have to implement my_Func
	glutMouseFunc(My_Mouse);

	// Enter main event loop.
	glutMainLoop();

	return 0;
}